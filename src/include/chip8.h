#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <fstream>
#include <chrono>

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

class chip8 {
public:
	chip8();

	void load_rom(const char*);
	void cycle();

	uint32_t _video[64 * 32]{ 0x000000FF };
	uint8_t  _keypad[16]{ 0 };

private:
	uint8_t  _memory[4096];
	uint8_t  _register[16];
	uint16_t _stack[16];
	uint16_t _pc;
	uint8_t  _sp;
	uint8_t  _delay_timer;
	uint8_t  _sound_timer;
	uint16_t _opcode;
	uint16_t _index;

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

	void Table0();
	void Table8();
	void TableE();
	void TableF();
	void OP_NULL() { }

	typedef void (chip8::* Chip8Func)();
	Chip8Func table[0xF + 1]{ &chip8::OP_NULL };
	Chip8Func table0[0xE + 1]{ &chip8::OP_NULL };
	Chip8Func table8[0xE + 1]{ &chip8::OP_NULL };
	Chip8Func tableE[0xE + 1]{ &chip8::OP_NULL };
	Chip8Func tableF[0x65 + 1]{ &chip8::OP_NULL };
};

chip8::chip8()
	: _pc(START_ADDRESS)
{
	for (size_t i = 0; i < FONTSET_SIZE; ++i) {
		_memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}

	// Set up function pointer table
	table[0x0] = &chip8::Table0;
	table[0x1] = &chip8::OP_1nnn;
	table[0x2] = &chip8::OP_2nnn;
	table[0x3] = &chip8::OP_3xkk;
	table[0x4] = &chip8::OP_4xkk;
	table[0x5] = &chip8::OP_5xy0;
	table[0x6] = &chip8::OP_6xkk;
	table[0x7] = &chip8::OP_7xkk;
	table[0x8] = &chip8::Table8;
	table[0x9] = &chip8::OP_9xy0;
	table[0xA] = &chip8::OP_Annn;
	table[0xB] = &chip8::OP_Bnnn;
	table[0xC] = &chip8::OP_Cxkk;
	table[0xD] = &chip8::OP_Dxyn;
	table[0xE] = &chip8::TableE;
	table[0xF] = &chip8::TableF;

	table0[0x0] = &chip8::OP_00E0;
	table0[0xE] = &chip8::OP_00EE;

	table8[0x0] = &chip8::OP_8xy0;
	table8[0x1] = &chip8::OP_8xy1;
	table8[0x2] = &chip8::OP_8xy2;
	table8[0x3] = &chip8::OP_8xy3;
	table8[0x4] = &chip8::OP_8xy4;
	table8[0x5] = &chip8::OP_8xy5;
	table8[0x6] = &chip8::OP_8xy6;
	table8[0x7] = &chip8::OP_8xy7;
	table8[0xE] = &chip8::OP_8xyE;

	tableE[0x1] = &chip8::OP_ExA1;
	tableE[0xE] = &chip8::OP_Ex9E;

	tableF[0x07] = &chip8::OP_Fx07;
	tableF[0x0A] = &chip8::OP_Fx0A;
	tableF[0x15] = &chip8::OP_Fx15;
	tableF[0x18] = &chip8::OP_Fx18;
	tableF[0x1E] = &chip8::OP_Fx1E;
	tableF[0x29] = &chip8::OP_Fx29;
	tableF[0x33] = &chip8::OP_Fx33;
	tableF[0x55] = &chip8::OP_Fx55;
	tableF[0x65] = &chip8::OP_Fx65;
}

void chip8::Table0()
{
	((*this).*(table0[_opcode & 0x000Fu]))();
}

void chip8::Table8()
{
	((*this).*(table8[_opcode & 0x000Fu]))();
}

void chip8::TableE()
{
	((*this).*(tableE[_opcode & 0x000Fu]))();
}

void chip8::TableF()
{
	((*this).*(tableF[_opcode & 0x00FFu]))();
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

		for (long i = 0; i < size; ++i) {
			_memory[START_ADDRESS + i] = static_cast<uint8_t>(buffer[i]);
		}
		delete[] buffer;
	}
}

void chip8::cycle()
{
	// Fetch
	_opcode = (_memory[_pc] << 8) | _memory[_pc + 1];

	// Increment the PC before we execute anything
	_pc += 2;

	// Decode and Execute
	((*this).*(table[(_opcode & 0xF000u) >> 12]))();

	// Decrement the delay timer if it's been set
	if (_delay_timer > 0)
		--_delay_timer;

	// Decrement the sound timer if it's been set
	if (_sound_timer > 0)
		--_sound_timer;
}

void chip8::OP_00E0()
{
	// Clear the display.
	memset(_video, 0x000000FF, sizeof(_video));
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
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t Vy = (_opcode & 0x00F0u) >> 4;

	if (_register[Vy] > _register[Vx])
		_register[0xF] = 1;
	else _register[0xF] = 0;

	_register[Vx] = _register[Vy] - _register[Vx];
}

void chip8::OP_8xyE()
{
	// Set Vx = Vx SHL 1.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	_register[0xF] = (_register[Vx] & 0x80u) >> 7;
	_register[Vx] <<= 1;
}

void chip8::OP_9xy0()
{
	// Skip next instruction if Vx != Vy.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t Vy = (_opcode & 0x00F0u) >> 4;

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

void chip8::OP_Cxkk()
{
	// Set Vx = random byte AND kk.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t byte = _opcode & 0x00FFu;
	_register[Vx] = static_cast<uint8_t>(rand()%256) & byte;
}

void chip8::OP_Dxyn()
{
	// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t Vy = (_opcode & 0x00F0u) >> 4;
	uint8_t height = _opcode & 0x000Fu;

	uint8_t xPos = _register[Vx] % 64;
	uint8_t yPos = _register[Vy] % 32;

	_register[0xF] = 0;

	for (size_t row = 0; row < height; ++row) {
		uint8_t spriteByte = _memory[_index + row];

		for (size_t col = 0; col < 8; ++col) {
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &_video[(31 - (yPos + row)) * 64 + (xPos + col)];

			if (spritePixel) {
				if (*screenPixel == 0xFFFFFFFF)
					_register[0xF] = 1;
				
				*screenPixel ^= 0xFFFFFF00;
			}
		}
	}
}

void chip8::OP_Ex9E()
{
	// Skip next instruction if key with the value of Vx is pressed.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8u;
	uint8_t key = _register[Vx];

	if (_keypad[key])
		_pc += 2;
}

void chip8::OP_ExA1()
{
	// Skip next instruction if key with the value of Vx is not pressed.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t key = _register[Vx];

	if (!_keypad[key])
		_pc += 2;
}

void chip8::OP_Fx07()
{
	// Set Vx = delay timer value.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	_register[Vx] = _delay_timer;
}

void chip8::OP_Fx0A()
{
	// Wait for a key press, store the value of the key in Vx.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;

	if (_keypad[0])
		_register[Vx] = 0;
	else if (_keypad[1])
		_register[Vx] = 1;
	else if (_keypad[2])
		_register[Vx] = 2;
	else if (_keypad[3])
		_register[Vx] = 3;
	else if (_keypad[4])
		_register[Vx] = 4;
	else if (_keypad[5])
		_register[Vx] = 5;
	else if (_keypad[6])
		_register[Vx] = 6;
	else if (_keypad[7])
		_register[Vx] = 7;
	else if (_keypad[8])
		_register[Vx] = 8;
	else if (_keypad[9])
		_register[Vx] = 9;
	else if (_keypad[10])
		_register[Vx] = 10;
	else if (_keypad[11])
		_register[Vx] = 11;
	else if (_keypad[12])
		_register[Vx] = 12;
	else if (_keypad[13])
		_register[Vx] = 13;
	else if (_keypad[14])
		_register[Vx] = 14;
	else if (_keypad[15])
		_register[Vx] = 15;
	else
		_pc -= 2;
}

void chip8::OP_Fx15()
{
	// Set delay timer = Vx.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	_delay_timer = _register[Vx];
}

void chip8::OP_Fx18()
{
	// Set sound timer = Vx.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	_sound_timer = _register[Vx];
}

void chip8::OP_Fx1E()
{
	// Set I = I + Vx.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	_index += _register[Vx];
}

void chip8::OP_Fx29()
{
	// Set I = location of sprite for digit Vx.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t digit = _register[Vx];
	_index = FONTSET_START_ADDRESS + (5 * digit);
}

void chip8::OP_Fx33()
{
	// Store BCD representation of Vx in memory locations I, I+1, and I+2.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;
	uint8_t value = _register[Vx];

	// Ones-place
	_memory[_index + 2] = value % 10;
	value /= 10;

	// Tens-place
	_memory[_index + 1] = value % 10;
	value /= 10;

	// Hundreds-place
	_memory[_index] = value % 10;
}

void chip8::OP_Fx55()
{
	// Store registers V0 through Vx in memory starting at location I.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;

	for (uint8_t i = 0; i <= Vx; ++i) {
		_memory[_index + i] = _register[i];
	}
}

void chip8::OP_Fx65()
{
	// Read registers V0 through Vx from memory starting at location I.
	uint8_t Vx = (_opcode & 0x0F00u) >> 8;

	for (uint8_t i = 0; i <= Vx; ++i) {
		_register[i] = _memory[_index + i];
	}
}

#endif // !CHIP8_H