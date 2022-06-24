#ifndef CHIP8_H
#define CHIP8_H

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int FONTSET_SIZE = 80;

uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

#include <cstdint>
#include <fstream>
#include <chrono>
#include <random>

class chip8 {
public:
	chip8();

	void load_rom(const char* filename);

	// Instructions
	void OP_00E0();
	void OP_00EE();
	void OP_1nnn();
	void OP_2nnn();
	void OP_3xkk();
	void OP_4xkk();
	void OP_5xy0();
	void OP_6xkk();
	void OP_7xkk();
	void OP_8xy0();
	void OP_8xy1();
	void OP_8xy2();
	void OP_8xy3();
	void OP_8xy4();
	void OP_8xy5();
	void OP_8xy6();
	void OP_8xy7();
	void OP_8xyE();
	void OP_9xy0();
	void OP_Annn();
	void OP_Bnnn();
	void OP_Cxkk();
	void OP_Dxyn();
	void OP_Ex9E();
	void OP_ExA1();
	void OP_Fx07();
	void OP_Fx0A();
	void OP_Fx15();
	void OP_Fx18();
	void OP_Fx1E();
	void OP_Fx29();
	void OP_Fx33();
	void OP_Fx55();
	void OP_Fx65();

private:
	uint8_t  _memory[4096];
	uint8_t  _register[16];
	uint16_t _stack[16];
	uint16_t _pc;
	uint8_t  _sp;
	uint8_t  _delay_timer;
	uint8_t  _sound_timer;
	uint8_t  _keypad[16];
	uint32_t _video[64 * 32];
	uint16_t _opcode;
	uint16_t _index;

	std::default_random_engine rand_gen;
	std::uniform_int_distribution<uint8_t> rand_byte;
};

chip8::chip8()
	: _pc(START_ADDRESS), rand_gen(std::chrono::system_clock::now().time_since_epoch().count())
{
	for (size_t i = 0; i < FONTSET_SIZE; ++i) {
		_memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}

	rand_byte = std::uniform_int_distribution<uint8_t>(0, 255U);
}

void chip8::load_rom(const char* filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open()) {
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		for (size_t i = 0; i < size; ++i) {
			_memory[START_ADDRESS + i] = buffer[i];
		}
		delete[] buffer;
	}
}

void chip8::OP_00E0()
{
	// Clear the display.
	memset(_video, 0, sizeof(_video));
}

void chip8::OP_00EE()
{
	// Return from a subroutine.
	_sp = _sp - 1;
	_pc = _stack[_sp];
}

void chip8::OP_1nnn()
{
	// Jump to location nnn.
	uint16_t address = _opcode & 0x0FFFu;
	_pc = address;
}

void chip8::OP_2nnn()
{
	// Call subroutine at nnn.
	uint16_t address = _opcode & 0x0FFFu;
	_stack[_sp] = _pc;
	_sp = _sp + 1;
	_pc = address;
}

void chip8::OP_3xkk()
{
	// Skip next instruction if Vx = kk.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8u;
	uint8_t byte = _opcode & 0x00FFu;

	if (_register[Vx] == byte)
		_pc += 2;
}

void chip8::OP_4xkk()
{
	// Skip next instruction if Vx != kk.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t byte = _opcode & 0x00FFu;

	if (_register[Vx] != byte)
		_pc += 2;
}

void chip8::OP_5xy0()
{
	// Skip next instruction if Vx = Vy.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t Vy = (_opcode & 0x00F0u) >> 4;

	if (_register[Vx] == _register[Vy])
		_pc += 2;
}

void chip8::OP_6xkk()
{
	// Set Vx = kk.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t byte = _opcode & 0x00FFu;
	_register[Vx] = byte;
}

void chip8::OP_7xkk()
{
	// Set Vx = Vx + kk.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t byte = _opcode & 0x00FFu;
	_register[Vx] += byte;
}

void chip8::OP_8xy0()
{
	// Set Vx = Vy.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t Vy = (_opcode & 0x00F0u) >> 4;
	_register[Vx] = _register[Vy];
}

void chip8::OP_8xy1()
{
	// Set Vx = Vx OR Vy.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t Vy = (_opcode & 0x00F0u) >> 4;
	_register[Vx] |= _register[Vy];
}

void chip8::OP_8xy2()
{
	// Set Vx = Vx AND Vy.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t Vy = (_opcode & 0x00F0u) >> 4;
	_register[Vx] &= _register[Vy];
}

void chip8::OP_8xy3()
{
	// Set Vx = Vx XOR Vy.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t Vy = (_opcode & 0x00F0u) >> 4;
	_register[Vx] ^= _register[Vy];
}

void chip8::OP_8xy4()
{
	// Set Vx = Vx + Vy, set VF = carry.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t Vy = (_opcode & 0x00F0u) >> 4;
	uint16_t sum = _register[Vx] + _register[Vy];

	if (sum > 255u)
		_register[0xF] = 1;
	else _register[0xF] = 0;
	
	_register[Vx] = sum & 0xFFu;
}

void chip8::OP_8xy5()
{
	// Set Vx = Vx - Vy, set VF = NOT borrow.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t Vy = (_opcode & 0x00F0u) >> 4;

	if (_register[Vx] > _register[Vy])
		_register[0xF] = 1;
	else _register[0xF] = 0;

	_register[Vx] -= _register[Vy];
}

void chip8::OP_8xy6()
{
	// Set Vx = Vx SHR 1.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	_register[0xF] = (_register[Vx] & 0x1u);
	_register[Vx] >>= 1;
}

void chip8::OP_8xy7()
{
	// Set Vx = Vy - Vx, set VF = NOT borrow.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (_opcode & 0x00F0u) >> 4u;

	if (_register[Vy] > _register[Vx])
		_register[0xF] = 1;
	else _register[0xF] = 0;

	_register[Vx] = _register[Vy] - _register[Vx];
}

void chip8::OP_8xyE()
{
	// Set Vx = Vx SHL 1.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8u;
	_register[0xF] = (_register[Vx] & 0x80u) >> 7u;
	_register[Vx] <<= 1;
}

void chip8::OP_9xy0()
{
	// Skip next instruction if Vx != Vy.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (_opcode & 0x00F0u) >> 4u;

	if (_register[Vx] != _register[Vy])
		_pc += 2;
}

void chip8::OP_Annn()
{
	// Set I = nnn.
	uint16_t address = _opcode & 0x0FFFu;
	_index = address;
}

void chip8::OP_Bnnn()
{
	// Jump to location nnn + V0.
	uint16_t address = _opcode & 0x0FFFu;
	_pc = _register[0] + address;
}

void chip8::OP_Cxkk();
{
	// Set Vx = random byte AND kk.
	
}

void chip8::OP_Dxyn();
{
	// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
}

void chip8::OP_Ex9E();
{
	// Skip next instruction if key with the value of Vx is pressed.
}

void chip8::OP_ExA1();
{
	// Skip next instruction if key with the value of Vx is not pressed.
}

void chip8::OP_Fx07();
{
	// Set Vx = delay timer value.
}

void chip8::OP_Fx0A();
{
	// Wait for a key press, store the value of the key in Vx.
}

void chip8::OP_Fx15();
{
	// Set delay timer = Vx.
}

void chip8::OP_Fx18();
{
	// Set sound timer = Vx.
}

void chip8::OP_Fx1E();
{
	// Set I = I + Vx.
}

void chip8::OP_Fx29();
{
	// Set I = location of sprite for digit Vx.
}

void chip8::OP_Fx33();
{
	// Store BCD representation of Vx in memory locations I, I+1, and I+2.
}

void chip8::OP_Fx55();
{
	// Store registers V0 through Vx in memory starting at location I.
}

void chip8::OP_Fx65();
{
	// Read registers V0 through Vx from memory starting at location I.
}

#endif // !CHIP8_H