#pragma once
namespace Hazel
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};
	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:   return 4;
			case ShaderDataType::Float2:  return 4 * 2;
			case ShaderDataType::Float3:  return 4 * 3;
			case ShaderDataType::Float4:  return 4 * 4;
			case ShaderDataType::Mat3:    return 4 * 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4 * 4;
			case ShaderDataType::Int:     return 4;
			case ShaderDataType::Int2:    return 4 * 2;
			case ShaderDataType::Int3:    return 4 * 3;
			case ShaderDataType::Int4:    return 4 * 4;
			case ShaderDataType::Bool:    return 1;
		}
		HZ_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;//名字字符串
		ShaderDataType Type;//数据的类型
		uint32_t Offset;//起始位置在第几个字节
		uint32_t Size;//这个数据类型的字节数
		bool Normalized;//是否归一化
		BufferElement()
		{
			
		}
		BufferElement(ShaderDataType type,const std::string& name, bool normalized = false)//构造函数这一步就已经有了数据类型的Size
			:Name(name),Type(type),Size(ShaderDataTypeSize(type)),Offset(0),Normalized(normalized)
		{
			
		}
		uint32_t GetComponentCount()const//计算每一组有多少个数据
		{
			switch (Type)
			{
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3 * 3;
				case ShaderDataType::Mat4:    return 4 * 4;
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;
			}
			HZ_CORE_ASSERT(false, "Unknown ShaderDataType");
			return 0;
		}
	};
	class BufferLayout
	{
	public:
		BufferLayout()
		{
			
		}
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			:m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}
		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements()const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin()const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end()const { return m_Elements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for(auto& element : m_Elements)
			{
				element.Offset = offset;//计算每一个layout的起始位置
				offset += element.Size;
				m_Stride += element.Size;//计算内存偏移量
			}
		}
	private:
		std::vector<BufferElement> m_Elements;//所有Layout
		uint32_t m_Stride = 0;//因为内存偏移量是所有Layout共用的，所以不需要在每一个Layout类(BufferElement类)里定义
	};
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual  ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;
		static IndexBuffer* Create(uint32_t* indices, uint32_t size);
		
	};
}


