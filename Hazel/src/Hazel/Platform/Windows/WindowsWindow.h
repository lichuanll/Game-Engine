#pragma
#include "Hazel/Window.h"

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
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		
		GLFWwindow* m_Window;

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
