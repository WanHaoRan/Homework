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

class hello : public contract {
	public:
		using contract::contract;

		[[eosio::action]]
		void hi( name user ){
			require_auth(_self);
			auto values=user.value;
			print("hello user value:",values);
			print(",:Hello,:",user);
			action(permission_level {_self,name("active")},
			name("games"), name("transfer"),
			std::make_tuple(_self,user,asset(1, symbol("WHR",4)),
				std::string("game send WHR")) ).send();
		}

		[[eosio::action]]
		void delay(std::string memo){
			eosio::transaction t{};
			t.actions.emplace_back(eosio::permission_level(_self,name("active")),
					name("gamedealer"),name("hi"),
				std::make_tuple(name("game")));
			t.delay_sec = 1;
			t.send(1,_self,false);
			print("delay end");
		}

		[[eosio::action]]
		void transfer(name from, name to, asset quantity, std::string memo){
			print("transfer from ",from);
		}

		[[eosio::action]]
		void add(name from,uint64_t phone){
			//构造函数 multi_index(uint64_t code, uint64_t scope)
			//参数1 code拥有表的账户，可以对数据库有读写权限
			//参数2 scope范围标识符，由于新版的CDT工具改变，scope应该改为_self.value
			work_index works(_self,_self.value);
			
			//emplace是添加一行数据到数据库表中
			//有两个参数，第一个是payer，为新对象所使用的存储付费的账户
			//第二个是constructor，lambda函数，可以让新创建的对象就地初始化
			works.emplace(_self, [&](auto& work){
				work.worker = from;
				work.phone = phone;
				work.score = asset(1, symbol("EOS",4));			
			});	

			
		}

		[[eosio::action]]
		void update(name from, uint64_t phone){
			//modify是使用主键修改表中的数据
			//参数itr： 待更新对象的迭代器
			//参数payer： 为更新数据付费的账户，为0则表示更新数据的payer和创建时的payer相同
			//参数updater： 用于更新目标对象的lambda函数
			//void modify(const_interator itr, uint64_t payer, lambda&& updater)

			//参数obj： 待更新对象的引用
			//void modify(const T& obj, uint64_t payer,lambda&& updater)
			
			work_index works(_self,_self.value);
			auto item=works.find(from.value);
			works.modify(item,_self,[&](auto& work){
				work.phone = phone;
			});
		}
		
		[[eosio::action]]
		void del(name from){
			
			work_index works(_self,_self.value);
			auto item = works.find(from.value);
			works.erase(item);
		}

		[[eosio::action]]
		void adds(name from){
			test_index t (_self,_self.value);
			t.emplace(_self,[&](auto& test){ test.id = t.available_primary_key();});
			print("add_end");		
		}

		[[eosio::action]]
		void query(uint64_t phone){
			work_index works(_self,_self.value);
			auto workitem=works.get_index<"phone"_n>();
			auto item=workitem.find(phone);
			print("query name is:",item->worker);
		}
		
		//@abi table content i64
		struct [[eosio::table]] work{	//这个eosio::table是生成合约的api接口使用
			name worker;	//姓名
			asset score;	//token数量
			uint64_t phone;	//电话
			//主键
			uint64_t primary_key() const {return worker.value;}
			//二级索引			
			uint64_t get_phone() const {return phone;}
			//序列化数据
			EOSLIB_SERIALIZE(work, (worker)(score)(phone))
		};
		//参数一： 表名称 最长12字符，只能由小写字母、数字1-5、“.”组成
		//参数二： 表对象 表中行的定义
		//参数三： 可变参数index 最多支持16个 必须返回结构中定义的常量类型，返回二级索引或者引用
		typedef eosio::multi_index<"work"_n, work ,
			indexed_by<"phone"_n, const_mem_fun<work, uint64_t, &work:: get_phone>>> work_index;

		struct [[eosio::table]] test{
			uint64_t id;
			uint64_t primary_key() const {return id;}
		};
		typedef eosio::multi_index<"test"_n, test > test_index;
			
};


#define EOSIO_DISPATCH_CUSTOM(TYPE, MEMBERS) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
   auto self = receiver; \
      if(( code == self&&action != name("transfer").value) || code == name("games").value && action == name("transfer").value) { \
        switch( action ) { \
            EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of this contract to run: eosio_exit(0); */ \
      } \
   } \
} \


EOSIO_DISPATCH(hello,(hi)(delay)(transfer)(add)(update)(del)(adds)(query))
