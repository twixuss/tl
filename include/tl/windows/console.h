static HANDLE consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

Printer console_printer = [](void const *string, umm length, Encoding encoding) {
	DWORD charsWritten;
	switch (encoding) {
		case Encoding_ascii: WriteConsoleA(consoleOutput, string, (DWORD)length, &charsWritten, 0); break;
		case Encoding_utf16: WriteConsoleW(consoleOutput, string, (DWORD)length, &charsWritten, 0); break;
	}
};

void clear_console() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT scrollRect;
	COORD scrollTarget;
	CHAR_INFO fill;

	// Get the number of character cells in the current buffer.
	if (!GetConsoleScreenBufferInfo(consoleOutput, &csbi))
	{
		return;
	}

	// Scroll the rectangle of the entire buffer.
	scrollRect.Left = 0;
	scrollRect.Top = 0;
	scrollRect.Right = csbi.dwSize.X;
	scrollRect.Bottom = csbi.dwSize.Y;

	// Scroll it upwards off the top of the buffer with a magnitude of the entire height.
	scrollTarget.X = 0;
	scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

	// Fill with empty spaces with the buffer's default text attribute.
	fill.Char.AsciiChar = ' ';
	fill.Attributes = csbi.wAttributes;

	// Do the scroll
	ScrollConsoleScreenBufferA(consoleOutput, &scrollRect, NULL, scrollTarget, &fill);

	// Move the cursor to the top left corner too.
	csbi.dwCursorPosition.X = 0;
	csbi.dwCursorPosition.Y = 0;

	SetConsoleCursorPosition(consoleOutput, csbi.dwCursorPosition);
}

DWORD get_code_page(Encoding encoding) {
	switch (encoding) {
		case Encoding_utf8: return 65001;
		case Encoding_utf16: return 1200;
	}
}

void set_console_mode(Encoding encoding) {
	auto cp = get_code_page(encoding);
	SetConsoleOutputCP(cp);
	SetConsoleCP(cp);
}
