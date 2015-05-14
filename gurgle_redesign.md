# LinkC Protocol   
## 简述    
LinkC 协议基于JSON（ 没有更多的说明了）    
## 启动连接(Start up connection)    
### 请求
```
"id"       : "id"    
"protocol" : "gurgle"   
"version"  : "unusable"   
"encrypt"  : "disabled"   
```
现在协议只支持 "gurgle"   
现在版本只存在 "unusable"   
现在加密只支持 "disabled"   
### 回应
```
"id"       : "request's id"    
"protocol" : "server's protocol"     
"version"  : "server's version"    
"encrypt"  : "encrypt status"     
```
如果协议或者版本不受支持，到后面disconnect部分会有说明   
一旦这三个字段检查完毕，CS连接正式建立，否则不算建立连接    
在未建立连接的情况下，发送任何其他字段的请求都会被强制断开物理连接    

## 检查延迟(ping)
### 请求
```
"id"       : "id"    
"cmd"      : "ping"    
```
目前设计不允许增添负载(payload)检测上下行网速
### 回应
```
"id"       : "request's id"    
"cmd"      : "pong"    
```

## 获取认证方式(Get authentication method)
### 请求
```
"id"       : "id"    
"cmd"      : "get"  w 
```