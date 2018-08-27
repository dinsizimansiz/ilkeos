#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>



namespace Customer
{
	using namespace eosio;
	using std::string;
	class Customers : public contract 
	{
		using contract::contract;
	public:
		Customers (account_name self) : contract(self){}

		//@abi action
		void addCustomer(const account_name name , const string firstName , const string secondName , uint64_t money)
		{
			require_auth(name);

			customerIndex customers(_self,_self);


			auto it = customers.find(name);

			eosio_assert(it == customers.end(),"Boyle bir musterimiz bulunmaktadir.");

			customers.emplace(name,[&] (auto& customer){
				customer.firstName = firstName;
				customer.surname = secondName;
				customer.balance = money;
			});
		}
		//@abi action
		void bagisgeldi(const account_name name , uint64_t bagisMiktari)
		{
			require_auth(name);

			customerIndex customers(_self,_self);

			auto it = customers.find(name);

			eosio_assert(it != customers.end(),"Boyle bir musterimiz yoktur.");

			customers.modify(it , name , [&](auto& customer){
					customer.balance += bagisMiktari;
				});	
		}
		//@abi action
		void  paragitti(const account_name name , uint64_t gidenPara)
		{
			require_auth(name);

			customerIndex customers(_self,_self);

			auto it = customers.find(name);

			eosio_assert(it != customers.end(),"Boyle bir musterimiz yoktur.");

			customers.modify(it , name , [&](auto& customer){
					customer.balance -= gidenPara;
				});	
		}
		//@abi action 
		void deleteCustomer(const account_name name )
		{
			require_auth(name);

			customerIndex customers(_self,_self);

			auto it = customers.find(name);

			eosio_assert(it != customers.end(),"Boyle bir musterimiz yoktur.");

			customers.erase(it);
		}
		//@abi action 
		void takas(const account_name sender,const account_name receiver,uint64_t amount)
		{

			

			require_auth(sender);

			customerIndex customers(_self,_self);

			auto it = customers.find(sender);

			eosio_assert(it != customers.end(),"Boyle bir musterimiz yoktur.");

			auto it2 = customers.find(receiver);

			eosio_assert(it2 != customers.end(),"Boyle bir musterimiz yoktur.");

			customers.modify(it , sender, [&](auto& sender){
				eosio_assert(sender.getBalance() - amount < 0,"Musterimiz para gonderemez." );
				sender.balance -= amount;
			});
			customers.modify(it2 , receiver ,[&](auto& receiver){
				receiver.balance += amount;
			});

			
		}


	private:
		//@abi table player i64
		struct Customer
		{
			uint64_t primaryKey;
			string firstName;
			string surname;
			uint64_t balance;

			uint64_t primary_key() const {return primaryKey;}
			string getFirstName() const {return firstName;}
			string getSurname() const {return surname;}
			uint64_t getBalance() const {return balance;}
			EOSIOLIB_SERIALIZE(Customer,(primaryKey)(firstName)(surname)(balance));
		};

		typedef multi_index<N(Customer),Customer> customerIndex;

	};

	EOSIO_ABI(Customers,(addCustomer)(bagisgeldi)(paragitti)(deleteCustomer)(takas));
}
