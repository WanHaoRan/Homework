一 目标
=====

1.创建一个叫game的用户。<br>
2.写一个游戏合约把第一课自己创建的token通过游戏合约转账给game用户。<br>
3.游戏合约只能由特定的用户调用转账。<br>

二 创建game用户
=====
cleos system newaccount eosio game EOS7JGsvG5jer8fwZRRdaqJxAPbiK6KDgLjFwtEjLQtKfiMENWriU EOS7JGsvG5jer8fwZRRdaqJxAPbiK6KDgLjFwtEjLQtKfiMENWriU --stake-net "1.0000 SYS" --stake-cpu "1.0000 SYS" --buy-ram-kbytes 5000000<br>
![创建game用户](http://www.baidu.com/img/bdlogo.gif)<br>

三 创建合约用户gamedealer
=====
cleos system newaccount eosio gamedealer EOS7JGsvG5jer8fwZRRdaqJxAPbiK6KDgLjFwtEjLQtKfiMENWriU EOS7JGsvG5jer8fwZRRdaqJxAPbiK6KDgLjFwtEjLQtKfiMENWriU --stake-net "1.0000 SYS" --stake-cpu "1.0000 SYS" --buy-ram-kbytes 5000000  
![创建game用户](http://www.baidu.com/img/bdlogo.gif)<br>
