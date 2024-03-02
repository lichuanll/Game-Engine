#pragma
#include "Hazel/Core/Window.h"
#include "Hazel/Renderer/GraphicsContext.h"
struct GLFWwindow;
namespace Hazel
{
	class WindowsWindow :public Window
	{
	public:
		WindowsWindow(const WindowProps& Props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return  m_Data.Width; };
		inline unsigned int GetHeight() const override { return m_Data.Height; };

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
		void SetVSync(bool bEnabled) override;
		bool IsVSync() const override;
		inline  virtual void* GetNativeWindow()const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;
		struct WindowData
		{
			std::string Title;
			int Width;
			int Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
