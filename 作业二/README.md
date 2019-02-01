一 目标
=====

1.创建一个叫game的用户。<br>
2.写一个游戏合约把第一课自己创建的token通过游戏合约转账给game用户。<br>
3.游戏合约只能由特定的用户调用转账。<br>

二 创建game用户
=====
`
cleos system newaccount eosio game EOS7JGsvG5jer8fwZRRdaqJxAPbiK6KDgLjFwtEjLQtKfiMENWriU EOS7JGsvG5jer8fwZRRdaqJxAPbiK6KDgLjFwtEjLQtKfiMENWriU --stake-net "1.0000 SYS" --stake-cpu "1.0000 SYS" --buy-ram-kbytes 5000000
`  
![创建game用户](http://www.baidu.com/img/bdlogo.gif)  

三 创建合约用户gamedealer
=====
`
cleos system newaccount eosio gamedealer EOS7JGsvG5jer8fwZRRdaqJxAPbiK6KDgLjFwtEjLQtKfiMENWriU EOS7JGsvG5jer8fwZRRdaqJxAPbiK6KDgLjFwtEjLQtKfiMENWriU --stake-net "1.0000 SYS" --stake-cpu "1.0000 SYS" --buy-ram-kbytes 5000000
`<br>
![创建gamedealer用户](http://www.baidu.com/img/bdlogo.gif)  

四 编写hello合约
=====
`
eh
`  

五 编译hello合约并部署hello合约到gamedealer用户上
=====
编译合约  
`
eosio-cpp -o hello.wasm hello.cpp --abigen
`  
![编译hello合约](http://www.baidu.com/img/bdlogo.gif)  
部署合约  
`
cleos set contract gamedealer /home/haoran/contracts/eosio.contracts/hello/ -p gamedealer@active
`  
![部署hello合约](http://www.baidu.com/img/bdlogo.gif)  

六 授权给gamedealer账户
=====
`
 cleos set account permission gamedealer active '{"threshold": 1,"keys": [{"key": "EOS7JGsvG5jer8fwZRRdaqJxAPbiK6KDgLjFwtEjLQtKfiMENWriU","weight": 1}],"accounts": [{"permission":{"actor":"gamedealer","permission":"eosio.code"},"weight":1}]}' owner -p gamedealer@owner
`  
![授权](http://www.baidu.com/img/bdlogo.gif)  

七 将自己创建的代币转给gamedealer和其他账户
=====
因为我第一节课创建自己的用户时使用的用户名是games，这节课使用的是game和gamedealer，有点容易混淆，这里提出。  
转给合约用户  
`
cleos push action games issue '["gamedealer","1000.0000 WHR","memo"]' -p games@active
`  
![授权](http://www.baidu.com/img/bdlogo.gif)   
转给测试用户  
`
cleos push action games issue '["test3","1000.0000 WHR","memo"]' -p games@active
`  
![授权](http://www.baidu.com/img/bdlogo.gif)  

八 运行合约
=====
使用已授权的合约用户来运行  
`
cleos push action gamedealer hi '["game"]' -p gamedealer@active
`
![合约用户运行](http://www.baidu.com/img/bdlogo.gif)  
使用未授权的测试用户来运行  
`
cleos push action gamedealer hi '["game"]' -p test3@active
`  
![非合约用户运行](http://www.baidu.com/img/bdlogo.gif)  
使用代币发行的用户运行  
`
cleos push action gamedealer hi '["game"]' -p games@active
`  
![代币发行用户运行](http://www.baidu.com/img/bdlogo.gif)  
延时调用  
`
cleos push action gamedealer delay '["memo"]' -p gamedealer@active
`<br>
![延时调用](http://www.baidu.com/img/bdlogo.gif)  
