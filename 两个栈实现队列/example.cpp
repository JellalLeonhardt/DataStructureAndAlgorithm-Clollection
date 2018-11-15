#include <iostream>
#include <stack>

template <class T>
class CQueue
{
  public:
	CQueue(void) = default;
	~CQueue(void) = default;

	void appendTail(const T &node);
	T deleteHead();

  private:
	std::stack<T> stack1;
	std::stack<T> stack2;
};

template <class T>
void CQueue<T>::appendTail(const T &node)
{
	stack1.push(node);
}

template <class T>
T CQueue<T>::deleteHead()
{
	if(stack2.empty()){
		while(!stack1.empty()){
			stack2.push(stack1.top());
			stack1.pop();
		}
	}
	T tmp = stack2.top();
	stack2.pop();
	return tmp;
}

int main()
{
	CQueue<int> que;
	que.appendTail(3);
	que.appendTail(4);
	std::cout<< que.deleteHead() <<std::endl;
	return 0;
}