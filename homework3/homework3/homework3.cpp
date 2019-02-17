#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/dispatcher.hpp>

using namespace eosio;

class [[eosio::contract]] homework3 : public contract {
	public:
		using contract::contract;
		
		//用来创建一个work表项
		[[eosio::action]]
		void add(name from){
			work_index works(_self,_self.value);

			works.emplace(_self, [&](auto& work){
				work.id = works.available_primary_key();
				work.worker = from;
				work.token = asset(0, symbol("EOS", 4));
				work.score = 0;
			});		
		}


		//用来删除work表中的一个表项
		[[eosio::action]]
		void del(uint64_t id){
			work_index works(_self,_self.value);

			auto item = works.find(id);
			works.erase(item);
		}

		//kpi action，用来通过id来发放EOS
		[[eosio::action]]
		void kpi(uint64_t id, uint64_t score){
			require_auth(_self);
			work_index works(_self,_self.value);
			
			auto item = works.find(id);

			works.modify(item,_self,[&](auto& work){
				work.score = score;
				work.token = asset(score*10*10000, symbol("EOS",4));
			});
			action(permission_level {_self,name("active")},
			name("eosio.token"), name("transfer"),
			std::make_tuple(_self,item->worker,asset(score*10*10000, symbol("EOS",4)),
				std::string("game send EOS")) ).send();
			
		}

		//@abi table work i64
		struct [[eosio::table]] work{
			uint64_t id;
			name worker;
			asset token;
			uint64_t score;
			uint64_t create_time = current_time();
			//主键
			uint64_t primary_key() const {return id;}
			
			//序列化数据
			EOSLIB_SERIALIZE(work,(id)(worker)(token)(score)(create_time))
		};
		typedef eosio::multi_index<"work"_n, work> work_index;		
};


#define EOSIO_DISPATCH_CUSTOM(TYPE, MEMBERS) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
   auto self = receiver; \
      if(( code == self&&action != name("transfer").value) ) { \
        switch( action ) { \
            EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of this contract to run: eosio_exit(0); */ \
      } \
   } \
} \


EOSIO_DISPATCH(homework3,(add)(del)(kpi))
