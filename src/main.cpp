#include <iostream>

int main(int argc, char* argv[])
{
	if (argc != 4) {
		std::cerr << "Usage: <path> <scale> <delay> <ROM>" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}