# mqttclient_example
## 说明
    其中 lib/libmqttclient.so 库文件以及 include目录下的头文件为https://github.com/jiejieTop/mqttclient项目生成
## 编译方法
    gcc -o main main.c src/log.c src/cJSON.c -lmqttclient -L./lib -I./include -lpthread -lm
