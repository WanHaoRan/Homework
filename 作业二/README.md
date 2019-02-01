一 目标
=====

1.创建一个叫game的用户。<br>
2.写一个游戏合约把第一课自己创建的token通过游戏合约转账给game用户。<br>
3.游戏合约只能由特定的用户调用转账。<br>

二 创建game用户
=====
```
cleos system newaccount eosio game EOS7JGsvG5jer8fwZRRdaqJxAPbiK6KDgLjFwtEjLQtKfiMENWriU EOS7JGsvG5jer8fwZRRdaqJxAPbiK6KDgLjFwtEjLQtKfiMENWriU --stake-net "1.0000 SYS" --stake-cpu "1.0000 SYS" --buy-ram-kbytes 5000000
```  
![创建game用户](https://github.com/WanHaoRan/Homework/tree/master/%E4%BD%9C%E4%B8%9A%E4%BA%8C/pictures/1.png)  

三 创建合约用户gamedealer
=====
```
cleos system newaccount eosio gamedealer EOS7JGsvG5jer8fwZRRdaqJxAPbiK6KDgLjFwtEjLQtKfiMENWriU EOS7JGsvG5jer8fwZRRdaqJxAPbiK6KDgLjFwtEjLQtKfiMENWriU --stake-net "1.0000 SYS" --stake-cpu "1.0000 SYS" --buy-ram-kbytes 5000000
```  
![创建gamedealer用户](https://github.com/WanHaoRan/Homework/tree/master/%E4%BD%9C%E4%B8%9A%E4%BA%8C/pictures/2.png)  

四 编写hello合约
=====
```
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


EOSIO_DISPATCH(hello,(hi)(delay)(transfer))
```  

五 编译hello合约并部署hello合约到gamedealer用户上
=====
编译合约  
```
eosio-cpp -o hello.wasm hello.cpp --abigen
```  
![编译hello合约](https://github.com/WanHaoRan/Homework/tree/master/%E4%BD%9C%E4%B8%9A%E4%BA%8C/pictures/3.png)  
部署合约  
```
cleos set contract gamedealer /home/haoran/contracts/eosio.contracts/hello/ -p gamedealer@active
```  
![部署hello合约](https://github.com/WanHaoRan/Homework/tree/master/%E4%BD%9C%E4%B8%9A%E4%BA%8C/pictures/4.png)  

六 授权给gamedealer账户
=====
```
 cleos set account permission gamedealer active '{"threshold": 1,"keys": [{"key": "EOS7JGsvG5jer8fwZRRdaqJxAPbiK6KDgLjFwtEjLQtKfiMENWriU","weight": 1}],"accounts": [{"permission":{"actor":"gamedealer","permission":"eosio.code"},"weight":1}]}' owner -p gamedealer@owner
```  
![授权](https://github.com/WanHaoRan/Homework/tree/master/%E4%BD%9C%E4%B8%9A%E4%BA%8C/pictures/5.png)  

七 将自己创建的代币转给gamedealer和其他账户
=====
因为我第一节课创建自己的用户时使用的用户名是games，这节课使用的是game和gamedealer，有点容易混淆，这里提出。  
转给合约用户  
```
cleos push action games issue '["gamedealer","1000.0000 WHR","memo"]' -p games@active
```  
![授权](https://github.com/WanHaoRan/Homework/tree/master/%E4%BD%9C%E4%B8%9A%E4%BA%8C/pictures/6.png)   
转给测试用户  
```
cleos push action games issue '["test3","1000.0000 WHR","memo"]' -p games@active
```  
![授权](https://github.com/WanHaoRan/Homework/tree/master/%E4%BD%9C%E4%B8%9A%E4%BA%8C/pictures/7.png)  

八 运行合约
=====
使用已授权的合约用户来运行  
```
cleos push action gamedealer hi '["game"]' -p gamedealer@active
```  
![合约用户运行](https://github.com/WanHaoRan/Homework/tree/master/%E4%BD%9C%E4%B8%9A%E4%BA%8C/pictures/8.png)  
使用未授权的测试用户来运行  
```
cleos push action gamedealer hi '["game"]' -p test3@active
```  
![非合约用户运行](https://github.com/WanHaoRan/Homework/tree/master/%E4%BD%9C%E4%B8%9A%E4%BA%8C/pictures/9.png)  
使用代币发行的用户运行  
```
cleos push action gamedealer hi '["game"]' -p games@active
```  
![代币发行用户运行](https://github.com/WanHaoRan/Homework/tree/master/%E4%BD%9C%E4%B8%9A%E4%BA%8C/pictures/10.png)  
延时调用  
```
cleos push action gamedealer delay '["memo"]' -p gamedealer@active
```  
![延时调用](https://github.com/WanHaoRan/Homework/tree/master/%E4%BD%9C%E4%B8%9A%E4%BA%8C/pictures/11.png)  
