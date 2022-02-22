#pragma once

namespace Utility {

	template <class T>
	class SmartPointer {

	private:
		T*    _data;
	public:
		SmartPointer(T* Value) : _data(Value) {}
		~SmartPointer() {
			delete _data;
		}

		T& operator* () {
			return *_data;
		}

		T* operator-> () {
			return _data;
		}
	};
}
