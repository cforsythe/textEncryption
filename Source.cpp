//Project by Christopher Forsythe, Daniel Deleon, and David Beach

#include "auint.h"//you mean this?
#include <string>
#include <fstream>

class Cryptography
{
public:
  arbritary_uint P;
  arbritary_uint G;
  arbritary_uint BobSecret;
  arbritary_uint AliceSecret;
  arbritary_uint K;
  arbritary_uint BobPublic;
  arbritary_uint AlicePublic;
  arbritary_uint EncryptedMessage;
  arbritary_uint InverseK;
  arbritary_uint DecryptedM;
  
	Cryptography()
	{
	arbritary_uint numbers;
	P.from_String("35201546659608842026088328007565866231962578784643756647773109869245232364730066609837018108561065242031153677"); //Prime				//we need the from string? that means "string to number" yes you need that method, thats the only way long numbers can be initialized
	//P.from_String("280829369862134719390036617067");
	G.from_String("3");//generator
	AliceSecret.from_String("23"); //random number between generator and Prime(Bob doesn't know)
	BobSecret.from_String("19");  //Bobs random number
	BobPublic =  G;               //A public number that bob shares with Alice
	BobPublic.powAssignUnderMod(BobSecret, P); 
	K = BobPublic;
	K.powAssignUnderMod(AliceSecret, P);
	InverseK.from_String("0");
	InverseK.inverse_modulo(K,P);
	}
  

arbritary_uint encryptmessage(char message)
	{
	arbritary_uint num;
	num.from_int(message);
	//cout << "NUM: " << num.toString() << endl;
	arbritary_uint lEncryptedMessage = num * K;
	lEncryptedMessage = lEncryptedMessage % P;
	return lEncryptedMessage;
	}
arbritary_uint decrypt(string message)
	{
	arbritary_uint num;
	num.from_String(message.c_str());
	arbritary_uint lDecryptedM = InverseK * num;
	lDecryptedM = lDecryptedM % P;
	return lDecryptedM;
	}

};

void main()
	{
	Cryptography enc;
	string s;
  	ifstream infile("text.txt");	

	/*char text[1000];
	arbritary_uint test = enc.encryptmessage("efgh");


	test.to_Ascii(text);
	cout << "TEST!: "<< text << endl;
	string decrypt = text;
	arbritary_uint product = enc.decrypt(decrypt);
	product.to_Ascii(text);
	cout << "Final: " << text <<endl;

	arbritary_uint testf = enc.encryptmessage("efgh");
	testf.to_Ascii(text);
	cout << "TEST!: "<< text << endl;
	 decrypt = text;
	arbritary_uint  productf = enc.decrypt(decrypt);
	productf.to_Ascii(text);
	cout << "Final: " << text <<endl;
	return;*/
	if (infile.is_open() == 0)
	{
		cout << "file error" << endl;
		return;
	}
	//text input

	ofstream outfile("crypt.txt");
	if (outfile.is_open() == 0)
	{
		cout << "file error" << endl;
		return;
	}
	//reading and encryption
	while (infile.eof() == 0)
	{
		//encrypt here
		getline(infile, s);
		char text;
		string output;
		char sub;
		
		int length = s.length();
		for(int i=0;i<=length;i++)
			{
			sub = s[i];
			arbritary_uint binum = enc.encryptmessage(sub);
			string encrypted=binum.toString();
			cout << "OUT: " << encrypted << endl;
			if(encrypted.size()>0)
      			outfile << encrypted<<endl;
			
			output = "";

			}	
			outfile << "\n";
	}

	infile.close();
	outfile.close();

	infile.open("crypt.txt");

	if (infile.is_open() == 0)
	{
		cout << "file error" << endl;
		return;
	}
	//decryption
	cout << "decryptiondecryptiondecryptiondecryptiondecryptiondecryptiondecryptiondecryptiondecryption" << endl;
	string finaltext = "";
	while (infile.eof() == 0)
	{
		arbritary_uint binum;
		getline(infile, s);
		binum = enc.decrypt(s);
		char c = binum.digits[0];
		cout << "DECR: "<< c << endl;
		finaltext += c;
	}
	cout << finaltext << endl;
	infile.close();
}
