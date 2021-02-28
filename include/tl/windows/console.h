static HANDLE console_output = GetStdHandle(STD_OUTPUT_HANDLE);

Printer console_printer = [](void const *string, umm length, Encoding encoding) {
	DWORD charsWritten;
	switch (encoding) {
		case Encoding_ascii: WriteConsoleA(console_output, string, (DWORD)length, &charsWritten, 0); break;
		case Encoding_utf8:  WriteConsoleA(console_output, string, (DWORD)length, &charsWritten, 0); break;
		case Encoding_utf16: WriteConsoleW(console_output, string, (DWORD)length, &charsWritten, 0); break;
	}
};

void clear_console() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT sr;
	COORD st;
	CHAR_INFO fill;

	// Get the number of character cells in the current buffer.
	if (!GetConsoleScreenBufferInfo(console_output, &csbi))
	{
		return;
	}

	// Scroll the rectangle of the entire buffer.
	sr.Left = 0;
	sr.Top = 0;
	sr.Right = csbi.dwSize.X;
	sr.Bottom = csbi.dwSize.Y;

	// Scroll it upwards off the top of the buffer with a magnitude of the entire height.
	st.X = 0;
	st.Y = (SHORT)(0 - csbi.dwSize.Y);

	// Fill with empty spaces with the buffer's default text attribute.
	fill.Char.AsciiChar = ' ';
	fill.Attributes = csbi.wAttributes;

	// Do the scroll
	ScrollConsoleScreenBufferA(console_output, &sr, NULL, st, &fill);

	// Move the cursor to the top left corner too.
	csbi.dwCursorPosition.X = 0;
	csbi.dwCursorPosition.Y = 0;

	SetConsoleCursorPosition(console_output, csbi.dwCursorPosition);
}

DWORD get_code_page(Encoding encoding) {
	switch (encoding) {
		case Encoding_ascii: return 1251;
		case Encoding_utf8: return 65001;
		case Encoding_utf16: return 1200;
	}
	invalid_code_path();
	return -1;
}

void set_console_mode(Encoding encoding) {
	auto cp = get_code_page(encoding);
	SetConsoleOutputCP(cp);
	SetConsoleCP(cp);
}
