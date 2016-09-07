#ifndef SIGNALSLOT_H
#define SIGNALSLOT_H
#include <list>
// �ź�/�� ʵ��

namespace UI
{
	enum slot_type
	{
		slot_function = 0,
		slot_method = 1,
	};

    // ȫ�ֺ���/��Ա�������࣬������signal����ͳһ����
    template<typename Return, typename... Args>
    class slot_base
    {
    public:
		virtual slot_type type() = 0;
        virtual Return invoke(Args... args) = 0;
        virtual void  release() = 0;
    };

    // ȫ�ֺ�����װ
    template<typename Return, typename... Args>
    class function_slot : public slot_base<Return, Args...>
    {
    public:
        typedef Return(*Function)(Args...);
		function_slot(Function f) : m_func(f) {
		}

		bool equal(Function f){
			return f == m_func;
		}
		virtual slot_type type() override { 
			return slot_function; 
		}
        virtual Return invoke(Args... args) override{
			return m_func(args...);
        }
        virtual void  release() override {
            delete this;
        }

    private:
		Function m_func;
    };

    // ��Ա������װ
    template<typename Return, typename Class, typename... Args>
    class method_slot : public slot_base<Return, Args...>
    {
    public:
        typedef Return(Class::*Function)(Args...);

        method_slot(Class* pThis, Function f) : 
                m_this(pThis), m_func(f) {
		}

		bool equal(Class* pThis, Function f){
			return pThis == m_this && f == m_func;
		}
		bool equal(Class* pThis){
			return pThis == m_this;
		}
		virtual slot_type type() override { 
			return slot_method; 
		}
        virtual Return invoke(Args... args) override {
            return (m_this->*m_func)(args...);
        }
        virtual void  release() override {
            delete this;
        }

    private:
        Function  m_func;
        Class* m_this;
    };



    // �źš��¼���װ��
	// Ϊ��ʹ�������࣬�ݲ�ȥ֧�ַ���ֵģ�塣���еĻص���������void����ֵ
	// ֧�ֶ����ӵİ汾multi connections
    template</*typename Return, */typename... Args>
	class mcsignal
    {
    public:
		~mcsignal()
        {
			disconnect_all();
        }

        // �ύ
		void emit(Args... args)
		{
			auto iter = m_connections.begin();
			decltype(iter) iterNext; 

			for (; iter != m_connections.end(); )
			{
				// �����ڻص����Ƴ��Լ�
				iterNext = iter;
				++iterNext;

				(*iter)->invoke(args...);

				iter = iterNext;
			}
		}

		
		/* �ݲ�ȥ���Ƿ���ֵ
		Return emit_return_last(Args... args)
        {
			Return ret;
            for (auto i : m_connections)
            {
				ret = i->invoke(args...);
            }

			return ret;
        }
		*/

		typedef void Return;  // ֻȥ֧��void����ֵ

        // ����һ����Ա����
        template<typename Class>
        void connect(Class* pThis, Return(Class::*pFunc)(Args...))
        {
            slot_base<Return, Args...>* p = new method_slot<Return, Class, Args...>(pThis, pFunc);
            m_connections.push_back(p);
        }

        // ����һ��ȫ�ֺ���
        void connect(Return(*pFunc)(Args...))
        {
            slot_base<Return, Args...>* p = new function_slot<Return, Args...>(pFunc);
            m_connections.push_back(p);
        }

		// �Ͽ�һ����Ա����
		template<typename Class>
		void disconnect(Class* pThis, Return(Class::*pFunc)(Args...))
		{
			auto iter = m_connections.begin();
			for (; iter != m_connections.end(); ++iter)
			{
				auto slot = *iter;
				if (slot->type() != slot_method)
					continue;

				if (static_cast<method_slot<Return, Class, Args...>*>(slot)
						->equal(pThis, pFunc))
				{
                    (*iter)->release();
					m_connections.erase(iter);
					break;
				}
			}
		}

		// �Ͽ�һ�������µ���������
		template<typename Class>
		void disconnect(Class* pThis)
		{
			auto iter = m_connections.begin();
			for (; iter != m_connections.end();)
			{
				auto slot = *iter;
				if (slot->type() == slot_method)
				{
					if (static_cast<method_slot<Return, Class, Args...>*>(slot)
						->equal(pThis))
					{
                        (*iter)->release();
						iter = m_connections.erase(iter);
						continue;
					}
				}

				++iter;
			}
		}

		// �Ͽ�һ��ȫ�ֺ���
		void disconnect(Return(*pFunc)(Args...))
		{
			auto iter = m_connections.begin();
			for (; iter != m_connections.end(); ++iter)
			{
				auto slot = *iter;
				if (slot->type() != slot_function)
					continue;

				if (static_cast<function_slot<Return, Args...>*>(slot)
						->equal(pFunc))
				{
                    (*iter)->release();
					m_connections.erase(iter);
					break;
				}
			}
		}

        void disconnect_all()
        {
            for (auto i : m_connections)
            {
                i->release();  // ��������ģ��ȥ�ͷ�
            }
            m_connections.clear();
        }

    protected:
        std::list<slot_base<Return, Args...>*>  m_connections;
    };

	// �����Ӱ汾����֧��һ���¼��ж�����ӣ����ڼ�Сһ��signal��Ա���ڴ��С��
	template</*typename Return, */typename... Args>
	class signal
	{
	public:
		~signal()
		{
			disconnect();
		}

		// �ύ
		void emit(Args... args)
		{
			if (m_connection)
				m_connection->invoke(args...);

		}

		typedef void Return;  // ֻȥ֧��void����ֵ

		// ����һ����Ա����
		template<typename Class>
		void connect(Class* pThis, Return(Class::*pFunc)(Args...))
		{
#ifdef _DEBUG
			if (m_connection)
			{
				DebugBreak();
			}
#endif
			disconnect();
			slot_base<Return, Args...>* p = new method_slot<Return, Class, Args...>(pThis, pFunc);
			m_connection = p;
		}

		// ����һ��ȫ�ֺ���
		void connect(Return(*pFunc)(Args...))
		{
	#ifdef _DEBUG
			if (m_connection)
			{
				DebugBreak();
			}
	#endif
			disconnect();
			slot_base<Return, Args...>* p = new function_slot<Return, Args...>(pFunc);
			m_connection = p;
		}

		void disconnect()
		{
			if (m_connection)
			{
				m_connection->release();
				m_connection = nullptr;
			}
		}

	protected:
		slot_base<Return, Args...>*  m_connection = nullptr;
	};

}

#endif