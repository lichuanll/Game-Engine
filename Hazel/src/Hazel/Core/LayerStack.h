#pragma once

#include "Hazel/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel
{
	// ������һ��vectorģ����Layer��ջ��vector[0]����ջ�ף�vector[last]����ջ���������������ͷ�����layer
	class HAZEL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		// lay������ͨ��layer��Overlayer��������Ļ�����layer
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