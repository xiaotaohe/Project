.PHONY:all
all:Sys_client Selecter_client mysql_test
mysql_test:mysql_test.cpp 
	g++ -std=c++11 $^ -o $@ -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient
Sys_client:Sys_client.cpp
	g++ -std=c++11 $^ -o $@ -ljson -pthread -levent
Selecter_client:Selecter_client.cpp 
	g++ -std=c++11 $^ -o $@ -ljson -pthread
.PHONY:clean
clean:
	rm -rf Sys_client Selecter_client
