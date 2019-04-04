#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <fstream>

std::string openFile()
{
	OPENFILENAME ofn;
	char szFile[260];
	HANDLE hf;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		hf = CreateFile(ofn.lpstrFile,
			GENERIC_READ,
			0,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);

		CloseHandle(hf);
	}

	

	return std::string(szFile);
}

int main()
{
	std::string file = openFile();

	size_t pos = file.find("\\");;
	while (pos != std::string::npos)
	{
		file.replace(file.begin()+pos, file.begin()+pos+1, "/");
		pos = file.find("\\");
	}

	std::ifstream inFile;
	inFile.open(file, std::ios::in);
	if (!inFile)
	{
		exit(1);
	}

	std::string data;

	while (inFile >> data);

	inFile.close();

	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Works!");
	sf::CircleShape circle(100.f);
	circle.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		window.draw(circle);
		window.display();
	}
}