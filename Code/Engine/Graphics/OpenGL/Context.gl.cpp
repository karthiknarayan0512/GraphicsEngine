#include "../Context.h"
#include "../../UserOutput/UserOutput.h"
#include "../../../External/OpenGlExtensions/OpenGlExtensions.h"
#include "../../Windows/Functions.h"

#include <sstream>
#include <cassert>

namespace eae6320
{
	namespace Graphics
	{
		namespace
		{
			HWND s_renderingWindow = NULL;

			HDC s_deviceContext = NULL;
			HGLRC s_openGlRenderingContext = NULL;
		}

		namespace Context
		{
			float getAspectRatio()
			{
				RECT renderingWindow;
				GetWindowRect(s_renderingWindow, &renderingWindow);

				float screenWidth = static_cast<float>(renderingWindow.right - renderingWindow.left);
				float screenHeight = static_cast<float>(renderingWindow.bottom - renderingWindow.top);

				return static_cast<float>(screenWidth / screenHeight);
			}

			bool CreateRenderingContext()
			{
				// A "device context" can be thought of an abstraction that Windows uses
				// to represent the graphics adaptor used to display a given window
				s_deviceContext = GetDC(s_renderingWindow);
				if (s_deviceContext == NULL)
				{
					eae6320::UserOutput::Print("Windows failed to get the device context");
					return false;
				}
				// Windows requires that an OpenGL "render context" is made for the window we want to use to render into
				{
					// Set the pixel format of the rendering window
					{
						PIXELFORMATDESCRIPTOR desiredPixelFormat = { 0 };
						{
							desiredPixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
							desiredPixelFormat.nVersion = 1;

							desiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
							desiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
							desiredPixelFormat.cColorBits = 32;
							desiredPixelFormat.cDepthBits = 16;
							desiredPixelFormat.iLayerType = PFD_MAIN_PLANE;
						}
						// Get the ID of the desired pixel format
						int pixelFormatId;
						{
							pixelFormatId = ChoosePixelFormat(s_deviceContext, &desiredPixelFormat);
							if (pixelFormatId == 0)
							{
								std::stringstream errorMessage;
								errorMessage << "Windows couldn't choose the closest pixel format: " << eae6320::GetLastWindowsError();
								eae6320::UserOutput::Print(errorMessage.str());
								return false;
							}
						}
						// Set it
						if (SetPixelFormat(s_deviceContext, pixelFormatId, &desiredPixelFormat) == FALSE)
						{
							std::stringstream errorMessage;
							errorMessage << "Windows couldn't set the desired pixel format: " << eae6320::GetLastWindowsError();
							eae6320::UserOutput::Print(errorMessage.str());
							return false;
						}
					}
					// Create the OpenGL rendering context
					s_openGlRenderingContext = wglCreateContext(s_deviceContext);
					if (s_openGlRenderingContext == NULL)
					{
						std::stringstream errorMessage;
						errorMessage << "Windows failed to create an OpenGL rendering context: " << eae6320::GetLastWindowsError();
						eae6320::UserOutput::Print(errorMessage.str());
						return false;
					}
					// Set it as the rendering context of this thread
					if (wglMakeCurrent(s_deviceContext, s_openGlRenderingContext) == FALSE)
					{
						std::stringstream errorMessage;
						errorMessage << "Windows failed to set the current OpenGL rendering context: " << eae6320::GetLastWindowsError();
						eae6320::UserOutput::Print(errorMessage.str());
						return false;
					}
				}

				return true;
			}

			bool Initialize(const HWND i_renderingWindow)
			{
				s_renderingWindow = i_renderingWindow;

				// Create an OpenGL rendering context
				if (!CreateRenderingContext())
				{
					return false;
				}

				// Load any required OpenGL extensions
				{
					std::string errorMessage;
					if (!OpenGlExtensions::Load(&errorMessage))
					{
						UserOutput::Print(errorMessage);
						return false;
					}
				}

				// Enable culling
				glEnable(GL_CULL_FACE);
				assert(glGetError() == GL_NO_ERROR);

				glEnable(GL_DEPTH_TEST);
				assert(glGetError() == GL_NO_ERROR);

				glDepthMask(GL_TRUE);
				assert(glGetError() == GL_NO_ERROR);

				glDepthFunc(GL_LEQUAL);
				assert(glGetError() == GL_NO_ERROR);

				return true;
			}

			void ClearBuffers()
			{
				// Every frame an entirely new image will be created.
				// Before drawing anything, then, the previous image will be erased
				// by "clearing" the image buffer (filling it with a solid color)
				{
					// Black is usually used
					glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
					assert(glGetError() == GL_NO_ERROR);
					// In addition to the color, "depth" and "stencil" can also be cleared,
					// but for now we only care about color
					const GLbitfield clearColor = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
					glClear(clearColor);
					assert(glGetError() == GL_NO_ERROR);
				}
			}

			void SwapBuffers()
			{
				// Everything has been drawn to the "back buffer", which is just an image in memory.
				// In order to display it, the contents of the back buffer must be swapped with the "front buffer"
				// (which is what the user sees)
				{
					BOOL result = SwapBuffers(s_deviceContext);
					assert(result != FALSE);
				}
			}

			bool Shutdown()
			{
				bool wereThereErrors = false;

				if (s_openGlRenderingContext != NULL)
				{
					if (wglMakeCurrent(s_deviceContext, NULL) != FALSE)
					{
						if (wglDeleteContext(s_openGlRenderingContext) == FALSE)
						{
							std::stringstream errorMessage;
							errorMessage << "Windows failed to delete the OpenGL rendering context: " << GetLastWindowsError();
							UserOutput::Print(errorMessage.str());
						}
					}
					else
					{
						std::stringstream errorMessage;
						errorMessage << "Windows failed to unset the current OpenGL rendering context: " << GetLastWindowsError();
						UserOutput::Print(errorMessage.str());
					}
					s_openGlRenderingContext = NULL;
				}

				if (s_deviceContext != NULL)
				{
					// The documentation says that this call isn't necessary when CS_OWNDC is used
					ReleaseDC(s_renderingWindow, s_deviceContext);
					s_deviceContext = NULL;
				}

				s_renderingWindow = NULL;

				return !wereThereErrors;
			}

			void BeginRender()
			{

			}

			void EndRender()
			{

			}
		}
	}
}