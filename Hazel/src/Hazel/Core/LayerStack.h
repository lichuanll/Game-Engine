#pragma once

#include "Hazel/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel
{
	// 这里用一个vector模拟了Layer的栈，vector[0]代表栈底，vector[last]代表栈顶，存放了离摄像头最近的layer
	class HAZEL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		// lay代表普通的layer，Overlayer代表离屏幕最近的layer
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*>m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}