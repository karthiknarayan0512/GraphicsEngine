#include "..\Effect.h"
#include "..\..\UserOutput\UserOutput.h"
#include "..\..\Windows\Functions.h"
#include "..\..\Math\cQuaternion.h"
#include "..\..\Math\cVector.h"
#include "..\..\Math\Functions.h"
#include "..\Context.h"

#include <sstream>
#include <fstream>
#include <cassert>

namespace eae6320
{
	namespace Graphics
	{
		struct sLogInfo
		{
			GLchar* memory;
			sLogInfo(const size_t i_size) { memory = reinterpret_cast<GLchar*>(malloc(i_size)); }
			~sLogInfo() { if (memory) free(memory); }
		};

		Effect::~Effect()
		{
			if (m_programID != 0)
			{
				glDeleteProgram(m_programID);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to delete the program: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					UserOutput::Print(errorMessage.str());
				}
				m_programID = 0;
			}
		}

		void Effect::SetTransforms(Math::cMatrix_transformation i_localToWorldTransform, Camera &i_Camera)
		{
			const GLboolean dontTranspose = false; // Matrices are already in the correct format
			const GLsizei uniformCountToSet = 1;
			glUniformMatrix4fv(m_LocalToWorldTransform, uniformCountToSet, dontTranspose, reinterpret_cast<const GLfloat*>(&i_localToWorldTransform));

			Math::cMatrix_transformation i_WorldToViewTransform;
			Math::cQuaternion worldToViewCameraOrientation;
			i_WorldToViewTransform = Math::cMatrix_transformation::CreateWorldToViewTransform(worldToViewCameraOrientation, i_Camera.getCameraPosition());
			glUniformMatrix4fv(m_WorldToViewTransform, uniformCountToSet, dontTranspose, reinterpret_cast<const GLfloat*>(&i_WorldToViewTransform));

			Math::cMatrix_transformation i_ViewToScreenTransform;
			float i_FOV_y = Math::ConvertDegreesToRadians(60);
			float i_aspectRatio = Context::getAspectRatio();
			i_ViewToScreenTransform = Math::cMatrix_transformation::CreateViewToScreenTransform(i_FOV_y, i_aspectRatio, 0.1F, 100.0F);
			glUniformMatrix4fv(m_ViewToScreenTransform, uniformCountToSet, dontTranspose, reinterpret_cast<const GLfloat*>(&i_ViewToScreenTransform));
		}

		bool Effect::LoadAndAllocateShaderProgram(const char* i_path, void*& o_shader, size_t& o_size, std::string* o_errorMessage)
		{
			bool wereThereErrors = false;

			// Load the shader source from disk
			o_shader = NULL;
			HANDLE fileHandle = INVALID_HANDLE_VALUE;
			{
				// Open the file
				{
					const DWORD desiredAccess = FILE_GENERIC_READ;
					const DWORD otherProgramsCanStillReadTheFile = FILE_SHARE_READ;
					SECURITY_ATTRIBUTES* useDefaultSecurity = NULL;
					const DWORD onlySucceedIfFileExists = OPEN_EXISTING;
					const DWORD useDefaultAttributes = FILE_ATTRIBUTE_NORMAL;
					const HANDLE dontUseTemplateFile = NULL;
					fileHandle = CreateFile(i_path, desiredAccess, otherProgramsCanStillReadTheFile,
						useDefaultSecurity, onlySucceedIfFileExists, useDefaultAttributes, dontUseTemplateFile);
					if (fileHandle == INVALID_HANDLE_VALUE)
					{
						wereThereErrors = true;
						if (o_errorMessage)
						{
							std::string windowsErrorMessage = eae6320::GetLastWindowsError();
							std::stringstream errorMessage;
							errorMessage << "Windows failed to open the shader file: " << windowsErrorMessage;
							*o_errorMessage = errorMessage.str();
						}
						goto OnExit;
					}
				}
				// Get the file's size
				{
					LARGE_INTEGER fileSize_integer;
					if (GetFileSizeEx(fileHandle, &fileSize_integer) != FALSE)
					{
						assert(fileSize_integer.QuadPart <= SIZE_MAX);
						o_size = static_cast<size_t>(fileSize_integer.QuadPart);
					}
					else
					{
						wereThereErrors = true;
						if (o_errorMessage)
						{
							std::string windowsErrorMessage = eae6320::GetLastWindowsError();
							std::stringstream errorMessage;
							errorMessage << "Windows failed to get the size of shader: " << windowsErrorMessage;
							*o_errorMessage = errorMessage.str();
						}
						goto OnExit;
					}
					// Add an extra byte for a NULL terminator
					o_size += 1;
				}
				// Read the file's contents into temporary memory
				o_shader = malloc(o_size);
				if (o_shader)
				{
					DWORD bytesReadCount;
					OVERLAPPED* readSynchronously = NULL;
					if (ReadFile(fileHandle, o_shader, o_size,
						&bytesReadCount, readSynchronously) == FALSE)
					{
						wereThereErrors = true;
						if (o_errorMessage)
						{
							std::string windowsErrorMessage = eae6320::GetLastWindowsError();
							std::stringstream errorMessage;
							errorMessage << "Windows failed to read the contents of shader: " << windowsErrorMessage;
							*o_errorMessage = errorMessage.str();
						}
						goto OnExit;
					}
				}
				else
				{
					wereThereErrors = true;
					if (o_errorMessage)
					{
						std::stringstream errorMessage;
						errorMessage << "Failed to allocate " << o_size << " bytes to read in the shader program " << i_path;
						*o_errorMessage = errorMessage.str();
					}
					goto OnExit;
				}
				// Add the NULL terminator
				reinterpret_cast<char*>(o_shader)[o_size - 1] = '\0';
			}

		OnExit:

			if (wereThereErrors && o_shader)
			{
				free(o_shader);
				o_shader = NULL;
			}
			if (fileHandle != INVALID_HANDLE_VALUE)
			{
				if (CloseHandle(fileHandle) == FALSE)
				{
					if (!wereThereErrors && o_errorMessage)
					{
						std::string windowsError = eae6320::GetLastWindowsError();
						std::stringstream errorMessage;
						errorMessage << "Windows failed to close the shader file handle: " << windowsError;
						*o_errorMessage = errorMessage.str();
					}
					wereThereErrors = true;
				}
				fileHandle = INVALID_HANDLE_VALUE;
			}

			return !wereThereErrors;
		}

		bool Effect::LoadFragmentShader(const char *i_fragmentShaderfile)
		{
			// Verify that compiling shaders at run-time is supported
			{
				GLboolean isShaderCompilingSupported;
				glGetBooleanv(GL_SHADER_COMPILER, &isShaderCompilingSupported);
				if (!isShaderCompilingSupported)
				{
					eae6320::UserOutput::Print("Compiling shaders at run-time isn't supported on this implementation (this should never happen)");
					return false;
				}
			}

			bool wereThereErrors = false;

			// Load the source code from file and set it into a shader
			GLuint fragmentShaderId = 0;
			void* shaderSource = NULL;
			{
				// Load the shader source code
				size_t fileSize;
				{
					const char* sourceCodeFileName = i_fragmentShaderfile;
					std::string errorMessage;
					if (!LoadAndAllocateShaderProgram(sourceCodeFileName, shaderSource, fileSize, &errorMessage))
					{
						wereThereErrors = true;
						eae6320::UserOutput::Print(errorMessage);
						goto OnExit;
					}
				}
				// Generate a shader
				fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
				{
					const GLenum errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to get an unused fragment shader ID: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
					else if (fragmentShaderId == 0)
					{
						wereThereErrors = true;
						eae6320::UserOutput::Print("OpenGL failed to get an unused fragment shader ID");
						goto OnExit;
					}
				}
				// Set the source code into the shader
				{
					const GLsizei shaderSourceCount = 1;
					const GLchar* shaderSources[] =
					{
						reinterpret_cast<GLchar*>(shaderSource)
					};
					const GLint* sourcesAreNullTerminated = NULL;
					glShaderSource(fragmentShaderId, shaderSourceCount, shaderSources, sourcesAreNullTerminated);
					const GLenum errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to set the fragment shader source code: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
			}
			// Compile the shader source code
			{
				glCompileShader(fragmentShaderId);
				GLenum errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					// Get compilation info
					// (this won't be used unless compilation fails
					// but it can be useful to look at when debugging)
					std::string compilationInfo;
					{
						GLint infoSize;
						glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoSize);
						errorCode = glGetError();
						if (errorCode == GL_NO_ERROR)
						{
							sLogInfo info(static_cast<size_t>(infoSize));
							GLsizei* dontReturnLength = NULL;
							glGetShaderInfoLog(fragmentShaderId, static_cast<GLsizei>(infoSize), dontReturnLength, info.memory);
							errorCode = glGetError();
							if (errorCode == GL_NO_ERROR)
							{
								compilationInfo = info.memory;
							}
							else
							{
								wereThereErrors = true;
								std::stringstream errorMessage;
								errorMessage << "OpenGL failed to get compilation info of the fragment shader source code: " <<
									reinterpret_cast<const char*>(gluErrorString(errorCode));
								eae6320::UserOutput::Print(errorMessage.str());
								goto OnExit;
							}
						}
						else
						{
							wereThereErrors = true;
							std::stringstream errorMessage;
							errorMessage << "OpenGL failed to get the length of the fragment shader compilation info: " <<
								reinterpret_cast<const char*>(gluErrorString(errorCode));
							eae6320::UserOutput::Print(errorMessage.str());
							goto OnExit;
						}
					}
					// Check to see if there were compilation errors
					GLint didCompilationSucceed;
					{
						glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &didCompilationSucceed);
						errorCode = glGetError();
						if (errorCode == GL_NO_ERROR)
						{
							if (didCompilationSucceed == GL_FALSE)
							{
								wereThereErrors = true;
								std::stringstream errorMessage;
								errorMessage << "The fragment shader failed to compile:\n" << compilationInfo;
								eae6320::UserOutput::Print(errorMessage.str());
								goto OnExit;
							}
						}
						else
						{
							wereThereErrors = true;
							std::stringstream errorMessage;
							errorMessage << "OpenGL failed to find out if compilation of the fragment shader source code succeeded: " <<
								reinterpret_cast<const char*>(gluErrorString(errorCode));
							eae6320::UserOutput::Print(errorMessage.str());
							goto OnExit;
						}
					}
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to compile the fragment shader source code: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
			// Attach the shader to the program
			{
				glAttachShader(m_programID, fragmentShaderId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to attach the fragment shader to the program: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}

		OnExit:

			if (fragmentShaderId != 0)
			{
				// Even if the shader was successfully compiled
				// once it has been attached to the program we can (and should) delete our reference to it
				// (any associated memory that OpenGL has allocated internally will be freed
				// once the program is deleted)
				glDeleteShader(fragmentShaderId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to delete the fragment shader ID: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
				}
				fragmentShaderId = 0;
			}
			if (shaderSource != NULL)
			{
				free(shaderSource);
				shaderSource = NULL;
			}

			return !wereThereErrors;
		}

		bool Effect::LoadVertexShader(const char *i_vertexShaderFile)
		{
			// Verify that compiling shaders at run-time is supported
			{
				GLboolean isShaderCompilingSupported;
				glGetBooleanv(GL_SHADER_COMPILER, &isShaderCompilingSupported);
				if (!isShaderCompilingSupported)
				{
					eae6320::UserOutput::Print("Compiling shaders at run-time isn't supported on this implementation (this should never happen)");
					return false;
				}
			}

			bool wereThereErrors = false;

			// Load the source code from file and set it into a shader
			GLuint vertexShaderId = 0;
			void* shaderSource = NULL;
			{
				// Load the shader source code
				size_t fileSize;
				{
					const char* sourceCodeFileName = i_vertexShaderFile;
					std::string errorMessage;
					if (!LoadAndAllocateShaderProgram(sourceCodeFileName, shaderSource, fileSize, &errorMessage))
					{
						wereThereErrors = true;
						eae6320::UserOutput::Print(errorMessage);
						goto OnExit;
					}
				}
				// Generate a shader
				vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
				{
					const GLenum errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to get an unused vertex shader ID: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
					else if (vertexShaderId == 0)
					{
						wereThereErrors = true;
						eae6320::UserOutput::Print("OpenGL failed to get an unused vertex shader ID");
						goto OnExit;
					}
				}
				// Set the source code into the shader
				{
					const GLsizei shaderSourceCount = 1;
					const GLchar* shaderSources[] =
					{
						reinterpret_cast<GLchar*>(shaderSource)
					};
					const GLint* sourcesAreNullTerminated = NULL;
					glShaderSource(vertexShaderId, shaderSourceCount, shaderSources, sourcesAreNullTerminated);
					const GLenum errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to set the vertex shader source code: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
			}
			// Compile the shader source code
			{
				glCompileShader(vertexShaderId);
				GLenum errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					// Get compilation info
					// (this won't be used unless compilation fails
					// but it can be useful to look at when debugging)
					std::string compilationInfo;
					{
						GLint infoSize;
						glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoSize);
						errorCode = glGetError();
						if (errorCode == GL_NO_ERROR)
						{
							sLogInfo info(static_cast<size_t>(infoSize));
							GLsizei* dontReturnLength = NULL;
							glGetShaderInfoLog(vertexShaderId, static_cast<GLsizei>(infoSize), dontReturnLength, info.memory);
							errorCode = glGetError();
							if (errorCode == GL_NO_ERROR)
							{
								compilationInfo = info.memory;
							}
							else
							{
								wereThereErrors = true;
								std::stringstream errorMessage;
								errorMessage << "OpenGL failed to get compilation info of the vertex shader source code: " <<
									reinterpret_cast<const char*>(gluErrorString(errorCode));
								eae6320::UserOutput::Print(errorMessage.str());
								goto OnExit;
							}
						}
						else
						{
							wereThereErrors = true;
							std::stringstream errorMessage;
							errorMessage << "OpenGL failed to get the length of the vertex shader compilation info: " <<
								reinterpret_cast<const char*>(gluErrorString(errorCode));
							eae6320::UserOutput::Print(errorMessage.str());
							goto OnExit;
						}
					}
					// Check to see if there were compilation errors
					GLint didCompilationSucceed;
					{
						glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &didCompilationSucceed);
						errorCode = glGetError();
						if (errorCode == GL_NO_ERROR)
						{
							if (didCompilationSucceed == GL_FALSE)
							{
								wereThereErrors = true;
								std::stringstream errorMessage;
								errorMessage << "The vertex shader failed to compile:\n" << compilationInfo;
								eae6320::UserOutput::Print(errorMessage.str());
								goto OnExit;
							}
						}
						else
						{
							wereThereErrors = true;
							std::stringstream errorMessage;
							errorMessage << "OpenGL failed to find out if compilation of the vertex shader source code succeeded: " <<
								reinterpret_cast<const char*>(gluErrorString(errorCode));
							eae6320::UserOutput::Print(errorMessage.str());
							goto OnExit;
						}
					}
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to compile the vertex shader source code: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
			// Attach the shader to the program
			{
				glAttachShader(m_programID, vertexShaderId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to attach the vertex shader to the program: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}

		OnExit:

			if (vertexShaderId != 0)
			{
				// Even if the shader was successfully compiled
				// once it has been attached to the program we can (and should) delete our reference to it
				// (any associated memory that OpenGL has allocated internally will be freed
				// once the program is deleted)
				glDeleteShader(vertexShaderId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to delete the vertex shader ID: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
				}
				vertexShaderId = 0;
			}
			if (shaderSource != NULL)
			{
				free(shaderSource);
				shaderSource = NULL;
			}

			return !wereThereErrors;
		}

		void Effect::SetEffect()
		{
			glUseProgram(m_programID);
			assert(glGetError() == GL_NO_ERROR);
		}

		bool Effect::CreateProgram(const char *i_vertexShaderFile, const char *i_fragmentShaderfile)
		{
			// Create a program
			{
				m_programID = glCreateProgram();
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to create a program: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					return false;
				}
				else if (m_programID == 0)
				{
					eae6320::UserOutput::Print("OpenGL failed to create a program");
					return false;
				}
			}
			// Load and attach the shaders
			if (!LoadVertexShader(i_vertexShaderFile))
			{
				return false;
			}
			if (!LoadFragmentShader(i_fragmentShaderfile))
			{
				return false;
			}
			// Link the program
			{
				glLinkProgram(m_programID);
				GLenum errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					// Get link info
					// (this won't be used unless linking fails
					// but it can be useful to look at when debugging)
					std::string linkInfo;
					{
						GLint infoSize;
						glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoSize);
						errorCode = glGetError();
						if (errorCode == GL_NO_ERROR)
						{
							sLogInfo info(static_cast<size_t>(infoSize));
							GLsizei* dontReturnLength = NULL;
							glGetProgramInfoLog(m_programID, static_cast<GLsizei>(infoSize), dontReturnLength, info.memory);
							errorCode = glGetError();
							if (errorCode == GL_NO_ERROR)
							{
								linkInfo = info.memory;
							}
							else
							{
								std::stringstream errorMessage;
								errorMessage << "OpenGL failed to get link info of the program: " <<
									reinterpret_cast<const char*>(gluErrorString(errorCode));
								eae6320::UserOutput::Print(errorMessage.str());
								return false;
							}
						}
						else
						{
							std::stringstream errorMessage;
							errorMessage << "OpenGL failed to get the length of the program link info: " <<
								reinterpret_cast<const char*>(gluErrorString(errorCode));
							eae6320::UserOutput::Print(errorMessage.str());
							return false;
						}
					}
					// Check to see if there were link errors
					GLint didLinkingSucceed;
					{
						glGetProgramiv(m_programID, GL_LINK_STATUS, &didLinkingSucceed);
						errorCode = glGetError();
						if (errorCode == GL_NO_ERROR)
						{
							if (didLinkingSucceed == GL_FALSE)
							{
								std::stringstream errorMessage;
								errorMessage << "The program failed to link:\n" << linkInfo;
								eae6320::UserOutput::Print(errorMessage.str());
								return false;
							}
						}
						else
						{
							std::stringstream errorMessage;
							errorMessage << "OpenGL failed to find out if linking of the program succeeded: " <<
								reinterpret_cast<const char*>(gluErrorString(errorCode));
							eae6320::UserOutput::Print(errorMessage.str());
							return false;
						}
					}
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to link the program: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					return false;
				}
			}

			m_LocalToWorldTransform = glGetUniformLocation(m_programID, "g_transform_localToWorld");
			m_WorldToViewTransform = glGetUniformLocation(m_programID, "g_transform_worldToView");
			m_ViewToScreenTransform = glGetUniformLocation(m_programID, "g_transform_viewToScreen");

			return true;
		}

		bool Effect::CreateEffect(const char *i_shaderBinaryFile)
		{
			std::ifstream effectBinary(i_shaderBinaryFile, std::ofstream::binary);

			effectBinary.seekg(0, effectBinary.end);
			int length = (int)effectBinary.tellg();
			effectBinary.seekg(0, effectBinary.beg);

			char * buffer = new char[length];
			effectBinary.read(buffer, length);

			char* i_vertexShaderFile = new char[strlen(buffer) + 1];
			memcpy_s(i_vertexShaderFile, strlen(buffer), buffer, strlen(buffer));
			i_vertexShaderFile[strlen(buffer)] = '\0';

			effectBinary.seekg(strlen(i_vertexShaderFile) + 1, effectBinary.beg);
			effectBinary.read(buffer, length - strlen(i_vertexShaderFile) - 1);

			char* i_fragmentShaderfile = new char[strlen(buffer) + 1];
			memcpy_s(i_fragmentShaderfile, strlen(buffer), buffer, strlen(buffer));
			i_fragmentShaderfile[strlen(buffer)] = '\0';

			effectBinary.close();

			return CreateProgram(i_vertexShaderFile, i_fragmentShaderfile);
		}
	}
}