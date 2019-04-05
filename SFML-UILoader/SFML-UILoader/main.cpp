#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <fstream>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <TGUI/TGUI.hpp>

std::string userSelectFile()
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
	std::string userChosenFile = userSelectFile();
	/*bool foundFile = false;
	do
	{
		userChosenFile = userSelectFile();

		if (userChosenFile.empty())
		{
			exit(1);
		}

		FILE* fb;
		errno_t err;
		if ((err = fopen_s(&fb, userChosenFile.c_str(), "rb")) == 0)
		{
			char readBuffer[65536];
			rapidjson::FileReadStream is(fb, readBuffer, sizeof(readBuffer));

			rapidjson::Document d;
			d.ParseStream(is);

			fclose(fb);

			if (!d.HasParseError())
			{
				foundFile = true;
			}
		}
	} while (!foundFile);
*/
	

	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Works!");
	tgui::Gui gui(window);
	gui.loadWidgetsFromFile(userChosenFile);
	sf::CircleShape circle(100.f);
	circle.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			gui.handleEvent(event);
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		window.draw(circle);
		gui.draw();
		window.display();
	}
}