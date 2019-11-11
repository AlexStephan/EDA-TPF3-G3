/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Client.h"

/*******************************************************************************
 * LOCAL FUNCTIONS DECLARATIONS
 ******************************************************************************/
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp);


/*******************************************************************************
	CONSTRUTORs
 ******************************************************************************/
Client::Client(Socket socket)
{
	curlm = curl_multi_init();
	curl = curl_easy_init();
	ip = socket.getIPString();
	port = socket.getPortString();
	running = 0;

}

Client::Client(NodeData data)
{
	curlm = curl_multi_init();
	curl = curl_easy_init();
	ip = data.getSocket().getIPString();
	port = data.getSocket().getPortString();
	running = 0;
}

/*******************************************************************************
	DESTRUCTOR
 ******************************************************************************/
Client::~Client()
{
	cout << "Destroyed Client" << endl;
	curl_multi_remove_handle(curlm, curl);
	curl_easy_cleanup(curl);
	curl_multi_cleanup(curlm);
}


/*******************************************************************************
	NETWORK CONFIGURATION
 ******************************************************************************/
void Client::POST(string path, string json)
{
	cType = POSTClient;
	string url = "http://127.0.0.1:" + port + path;

	if (curl && curlm)
	{
		//Le decimos a cURL que imprima todos los mensajes que se envían entre cliente y servidor.
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		//Seteamos primero la pagina donde nos vamos a conectar.
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		// Le decimos a cURL que trabaje con HTTP.
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);

		//Le decimos a cURL que cuando haya que escribir llame a myCallback
		//y que use al string readString como user data.
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		//Seteo para seguir el redireccionamiento
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)(json.size()) + 1);
		cout << json << endl; //DEBUG
		curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, json.c_str());

		curl_multi_add_handle(curlm, curl);
	}
}


void Client::GET(string path, string json)
{
	cType = GETClient;
	string url = "http://127.0.0.1:" + port + path;

	if (curl && curlm)
	{
		//Le decimos a cURL que imprima todos los mensajes que se envían entre cliente y servidor.
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		//Seteamos primero la pagina donde nos vamos a conectar.
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		// Le decimos a cURL que trabaje con HTTP.
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);

		//Le decimos a cURL que cuando haya que escribir llame a myCallback
		//y que use al string readString como user data.
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		//Seteo para seguir el redireccionamiento
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		struct curl_slist* header = NULL;
		header = curl_slist_append(header, json.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);

		curl_multi_add_handle(curlm, curl);

		//curl_slist_free_all(header);
	}

}


/*******************************************************************************
	COMMUNICATION
 ******************************************************************************/
errorType Client::sendRequest(void)
{
	errorType err = { false, "OK Client" };
	CURLMcode res;
	res = curl_multi_perform(curlm, &running);
	if (res != CURLM_OK)
	{
		err = { true, "Connection error" };
	}

	return err;
}


/*******************************************************************************
	GETTERS
 ******************************************************************************/
string Client::getResponse(void) { return response; }
int Client::getRunning() { return running; }
ClientType Client::getClientType() { return cType; }

Socket Client::getSocket() 
{
	Socket sock(atoi(port.c_str()),crakIp()) ;
	return sock;
}


ip_t Client::crackIp()
{
	ip_t _ip;
	size_t pos1 = ip.find('.');
	_ip.b1 = atoi(ip.substr(0, pos1).c_str());

	size_t pos2 = ip.find('.', pos1 + 1);
	_ip.b2 = atoi(ip.substr(pos1 + 1, pos2).c_str());

	size_t pos3 = ip.find('.', pos2 + 1);
	_ip.b3 = atoi(ip.substr(pos2 + 1, pos3).c_str());

	_ip.b4 = atoi(ip.substr(pos3 + 1).c_str());

	return _ip;
}


/*******************************************************************************
 * LOCAL FUNCTIONS DEFINITIONS
 ******************************************************************************/
 //Concatena lo recibido en content a s

static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	char* response = (char*)contents;
	string* data = (string*)userp;
	data->append(response, size * nmemb);

	return realsize;						//Recordar siempre devolver realsize
}

