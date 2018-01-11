//#include <chrono>
//#include <iostream>
//
//int TestFunction() {
//	int x = 0;
//	for (int i = 0; i < 99999999; i++) {
//		x *= 2;
//	}
//	return 0;
//}
//
//int main()
//{
//	auto start = std::chrono::system_clock::now();
//	TestFunction();
//	auto end = std::chrono::system_clock::now();
//	std::chrono::duration<double> elapsedSeconds = end - start;
//	std::cout << "Function took " << elapsedSeconds.count() << " seconds\n";
//}