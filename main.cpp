#include <string>
#include <random>
#include <iostream>
#include <fstream>

std::string alf = "abcdefghijklmnopqrstuvwxyz";
std::string qwerAlf = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void qwerText(std::string name)
{
	std::ifstream file(name);
	std::string textbook;

	while (!file.eof())
	{
		char buff;
		file.get(buff);
		if (qwerAlf.find(buff, 0) != std::string::npos)
		{
			textbook += buff;
		}
	}

	file.close();

	std::ofstream newFile("test.txt");
	newFile << textbook;
	newFile.close();
}

std::vector<double> calcFreqChar(std::string textbooka)
{
	std::vector<double> result(alf.size());

	unsigned long charCount = 0;
	while (!textbooka.empty())
	{
		//charCount++;
		for (int i = 0; i < alf.size(); ++i)
		{
			if (textbooka.back() == alf[i])
			{
				result[i]++;
				charCount++;
				break;
			}
		}
		textbooka.pop_back();
	}

	double test = 0;
	for (int i = 0; i < result.size(); ++i)
	{
		result[i] /= charCount;
		std::cout << alf[i] << " " << result[i] << "\n";
		test += result[i];
	}
	std::cout << "sum=" << test << "\n\n";
	return result;
}

void decryption(std::string name)
{
	std::ifstream book("book.txt");
	std::string textbook;

	while (!book.eof())
	{
		std::string buff;
		book >> buff;
		textbook += buff;
	}

	book.close();

	std::vector<double> freqChar = calcFreqChar(textbook);

	std::ifstream cryp(name);
	std::string textcryp;
	//cryp >> textcryp;
	while (!cryp.eof())
	{
		std::string buff;
		cryp >> buff;
		textcryp += buff;
	}
	cryp.close();

	std::vector<double> freqCrypChar = calcFreqChar(textcryp);

	/*for (int i = 0; i < freqChar.size(); ++i)
	{
		std::cout << alf[i] << " " << freqChar[i] << " " << freqCrypChar[i] << "\n";
	}*/
	std::cout << "\n";

	std::string freqAlf = alf;
	for (int i = 0; i < freqChar.size(); ++i)
	{
		for (int j = freqChar.size() - 1; j > i; --j)
		{
			if (freqChar[j - 1] > freqChar[j])
			{
				double buffD = freqChar[j - 1];
				freqChar[j - 1] = freqChar[j];
				freqChar[j] = buffD;

				char buffC = freqAlf[j - 1];
				freqAlf[j - 1] = freqAlf[j];
				freqAlf[j] = buffC;
			}
		}
	}

	std::string crypAlf = alf;
	for (int i = 0; i < freqCrypChar.size(); ++i)
	{
		for (int j = freqCrypChar.size() - 1; j > i; --j)
		{
			if (freqCrypChar[j - 1] > freqCrypChar[j])
			{
				double buffD = freqCrypChar[j - 1];
				freqCrypChar[j - 1] = freqCrypChar[j];
				freqCrypChar[j] = buffD;

				char buffC = crypAlf[j - 1];
				crypAlf[j - 1] = crypAlf[j];
				crypAlf[j] = buffC;
			}
		}
	}

	for (int i = 0; i < freqChar.size(); ++i)
	{
		std::cout << freqAlf[i] << " " << freqChar[i] << " " << freqCrypChar[i] << " " << crypAlf[i] << "\n";
	}
	
	for (int i = 0; i < crypAlf.size(); ++i)
	{
		std::cout << freqAlf[i] << " " << crypAlf[i] << "\n";
	}
	std::ofstream fout("decryp.txt");	
	for (int i = 0; i < textcryp.size(); ++i)
	{
		for (int j = 0; j < crypAlf.size(); ++j)
		{
			if (textcryp[i] == crypAlf[j])
			{
				fout << freqAlf[j];
			}
		}
	}
	fout.close();
}

std::string getNewAlf(std::string alfa, std::string keya)
{
	std::string result = keya;

	for (int i = 0; i < alfa.size(); ++i)
	{
		bool flag = true;
		for (int j = 0; j < keya.size(); ++j)
		{
			if (alfa[i] == keya[j])
			{
				flag = false;
				break;
			}
		}

		if (flag)
		{
			result.push_back(alfa[i]);
		}
	}

	//std::cout << "getNewAlf=" << result << "\n";
	return result;
}

std::string generateKeyWord(std::string alfa, int KeySizea)
{
	std::string result;
	result.resize(KeySizea);
	
	for (int i = 0; i < KeySizea; ++i)
	{
		result[i] = alfa[rand() % 25];
	}

	return result;
}

void cypherText(std::string texta)
{
	std::ifstream book(texta);
	std::string textbook;
	while (!book.eof())
	{
		std::string buff;
		book >> buff;
		textbook += buff;
	}
	book.close();

	int keySize = 3;
	std::string key = generateKeyWord(alf, keySize);

	//std::cout << "key=" << key << "\n";

	std::string sAlf = getNewAlf(alf, key);

	std::string result;
	for (int j = 0; j < textbook.size(); ++j)
	{
		for (int i = 0; i < alf.size(); ++i)
		{
			if (textbook[j] == alf[i])
			{
				result.push_back(sAlf[i]);
			}
		}
	}

	//std::cout << "cypherText=" << result << "/n";

	std::ofstream fout("cryp.txt");
	fout << result;
	fout.close();
}

int main()
{
	//qwerText("book.txt");

	cypherText("mytext.txt");

	decryption("cryp.txt");

	system("pause");
}