#include <iostream>
#include <Windows.h>

int main()
{
	bool running = true;
	
	// infinite loop until running is set to false
	while (running)
	{
		// listening for the END key to set to false
		if (GetAsyncKeyState(VK_END) & 1)
			running = false;

		std::cout << IsDebuggerPresent() << '\n'; // printing if debugger is present

		Sleep(50);
	}

	return 0;
}