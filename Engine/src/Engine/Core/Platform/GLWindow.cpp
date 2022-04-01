#include "GLWindow.h"

#include "Engine/Core/Events/ApplicationEvent.h"
#include "Engine/Core/Events/KeyEvent.h"
#include "Engine/Core/Events/MouseEvent.h"

namespace Engine {

    static bool s_GLFWInitialized = false;

    Window* Window::Create(const WindowProps &props) {
        return new GLWindow(props);
    }

    GLWindow::GLWindow(const WindowProps &props) {
        Init(props);
    }

    GLWindow::~GLWindow() {
        ShutDown();
    }

    void GLWindow::Init(const WindowProps &data) {
        m_Data.Title = data.Title;
        m_Data.Width = data.Width;
        m_Data.Height = data.Height;

        if (!glfwInit()) {
            printf("Could not initialize GLFW!\n");
            glfwTerminate();
            return;
        }

        s_GLFWInitialized = true;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
        if (m_Window == nullptr) {
            printf("There was an error creating a GLFWWindow!\n");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_Window);
        glfwShowWindow(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        glfwSwapInterval(1);

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            return;
        }

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(m_Window, &screenWidth, &screenHeight);

        // Define viewport
        glViewport(0, 0, screenWidth, screenHeight);

        // Events
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.callBackFn(event);

            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

            WindowCloseEvent event;
            data.callBackFn(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent keyPressed(key, 0);
                    data.callBackFn(keyPressed);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent keyReleased(key);
                    data.callBackFn(keyReleased);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyTypedEvent keyTyped(key);
                    data.callBackFn(keyTyped);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
            KeyTypedEvent keyTyped(codepoint);
            data.callBackFn(keyTyped);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent pressedEvent(button);
                    data.callBackFn(pressedEvent);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent releasedEvent(button);
                    data.callBackFn(releasedEvent);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
            MouseScrolledEvent scrolledEvent(xoffset, yoffset);
            data.callBackFn(scrolledEvent);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
            WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
            MouseMovedEvent movedEvent(xpos, ypos);
            data.callBackFn(movedEvent);
        });

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
    }

    void GLWindow::Update() {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void GLWindow::ShutDown() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    glm::mat4 GLWindow::CalcProjMatrix(float FOV, float Z_NEAR, float Z_FAR) {
        float aspectRatio = (float) GetWidth() / (float) GetHeight();
        return glm::perspective(FOV, aspectRatio, Z_NEAR, Z_FAR);
    }

}