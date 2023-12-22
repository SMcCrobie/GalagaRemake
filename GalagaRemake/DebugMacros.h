#pragma once
#include <Windows.h>
//#define Seans_Debug
#ifdef Seans_Debug
inline int allocation_counter = 0;

inline void* operator new(size_t size)
{
	//std::cout << "allocating  " << size << " bytes of memory" << std::endl;
	allocation_counter++;
	std::cout << "allocating, current counter: " << allocation_counter << std::endl;
	/*if (size > 1000)
		std::cout << "why???" << std::endl;*/
	void* p = malloc(size);
	return p;
}

inline void operator delete(void* p)
{
	//std::cout << "Deleting Memory" << std::endl;
	allocation_counter--;
	std::cout << "Deleting, current counter: " << allocation_counter << std::endl;
	free(p);
}

inline void* operator new[](size_t size)
{
	//std::cout << "allocating  " << size << " bytes of memory" << std::endl;
	/*if (size > 1000)
		std::cout << "why???" << std::endl;*/
	allocation_counter++;
	std::cout << "allocating, current counter: " << allocation_counter << std::endl;
	void* p = malloc(size);
	return p;
}

inline void operator delete[](void* p)
{
	allocation_counter--;
	std::cout << "Deleting, current counter: " << allocation_counter << std::endl;
	//std::cout << "Deleting Memory" << std::endl;
	free(p);
}

#endif // Seans_Debug

inline void ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}
inline void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}