#include <iostream>
#include <pqxx/pqxx>
#include <Windows.h>
#include <string>
#include <tuple>


struct dbclient {
	std::string name;
	std::string surename;
	std::string email;
};
struct findedclient {
	int id;
	std::string name;
	std::string surename;
	std::string email;
};

class DataBase {
private:
	pqxx::connection* con;
public:
	DataBase(const std::string& connect) {
		con = new pqxx::connection(connect);
		std::cout << "Connected to database successfully!" << std::endl;
	}
	
	void creating_tables() {
		pqxx::work transactioncreatedb(*con);
		transactioncreatedb.exec(
			"CREATE TABLE public.clients ("
			"id serial NOT NULL,"
			"name text NOT NULL, "
			"surename text NOT NULL,"
			"email text NULL,"
			"CONSTRAINT clients_pk PRIMARY KEY(id)"
			");"
			"CREATE TABLE public.phonenumbers("
			"id serial NOT NULL,"
			"number text NOT NULL,"
			"clientid int REFERENCES clients(id) NOT NULL,"
			"CONSTRAINT telnumbers_pk PRIMARY KEY(id)"
			"); ");
		transactioncreatedb.commit();
	}

	void addclient(dbclient client) {
		//int added
		pqxx::work transaction(*con);
		std::string command = "INSERT INTO client (name, surename, email) VALUES ('" + client.name + "', '"
			+ client.surename + "', '" + client.email + "')";
		std::cout << "addclient command: " << command << std::endl;
		transaction.exec(command);
		transaction.commit();
		std::cout << "Client added!" << std::endl;
		//add return id
	}

	void addphone(std::string phone, std::string clientid) {
		pqxx::work transaction(*con);
		std::string command = "INSERT INTO phonenumbers (number, clientid) VALUES ('" + phone + "', " + clientid + ")";
		std::cout << "phone add command: " << command << std::endl;
		transaction.exec(command);
		transaction.commit();
		std::cout << "Phone added!" << std::endl;
	}

	void changeclientinfo(int clientid, std::string infotype, std::string newinfo) {
		pqxx::work transaction(*con);

		std::string command = "UPDATE client SET "+ infotype + " = '" + newinfo + "' WHERE id = " + std::to_string(clientid);
		std::cout << "update info command: " << command << std::endl;
		transaction.exec(command);
		transaction.commit();
		std::cout << "Info sucsessfuly updated!" << std::endl;
	}

	void deletephone(int clientid) {
		pqxx::work transaction(*con);
		std::string command = "DELETE from phonenumbers WHERE clientid = " + clientid;
		std::cout << "Delete phone command: " << command << std::endl;
		transaction.exec(command);
		transaction.commit();
		std::cout << "Phone deleted!" << std::endl;
	}

	void deleteclient(int clientid) {
		pqxx::work transaction(*con);
		std::string command = "DELETE from phonenumbers WHERE clientid = " + clientid;
		std::cout << "Delete phone command: " << command << std::endl;
		transaction.exec(command);
		transaction.commit();
		std::cout << "Phone deleted!" << std::endl;

		std::string command = "DELETE from clients WHERE id = " + clientid;
		std::cout << "Delete client command: " << command << std::endl;
		transaction.exec(command);
		transaction.commit();
		std::cout << "Client deleted!" << std::endl;
	}

	void clientfind(std::string searchtype, std::string name) {
		if (searchtype == "number") {
			pqxx::work transaction(*con);
			std::string command = "SELECT clientid FROM phonenumbers WHERE " + searchtype + " = " + name;
			std::cout << "Find command: " << command << std::endl;
			auto clientfindid = transaction.query<int>(command);
			//не понимаю как преобразовать в int чтобы потом преобразовать в string

			std::string command = "SELECT id, name, surename, email FROM clients WHERE id = " + clientfindid;
			std::cout << "Find command: " << command << std::endl;
			
			auto record = transaction.query<int, std::string, std::string, std::string>(command);
				std::cout << std::get<0>(record) << std::get<1>(record) << std::get<2>(record) << std::get<3>(record);
				//не понимаю почему у меня get не работает с кортежем record
		}
		else {
			pqxx::work transaction(*con);
			std::string command = "SELECT id, name, surename, email FROM clients WHERE " + searchtype + " = " + name;
			std::cout << "Find command: " << command << std::endl;
			
			auto record = transaction.query<int, std::string, std::string, std::string>(command);
			std::cout << std::get<0>(record) << std::get<1>(record) << std::get<2>(record) << std::get<3>(record);
		}
		std::cout << "Client Founded!: " << std::endl;
	}
};



int main() {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	try {
		std::string connectinf = "host=127.0.0.1 port=5432 dbname=homework12.5 user=postgres password=Karandashik133";
		std::cout << connectinf << std::endl;

		DataBase Clients(connectinf);

		Clients.creating_tables();
		std::cout << "Table creation sucsessful!" << std::endl;


	}
	catch (const std::exception& e) {
		std::cout << "Exeption happened: " << e.what() << std::endl;
	}

}