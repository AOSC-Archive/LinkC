# LinkC Protocol   
## 简述    
LinkC 协议基于JSON（ 没有更多的说明了）    
## 启动连接(Start up connection)    
### 请求
```
"id"       : "id",    
"protocol" : "gurgle",   
"version"  : "unusable",   
"encrypt"  : "disabled"   
```
现在协议只支持 "gurgle"   
现在版本只存在 "unusable"   
现在加密只支持 "disabled"   
### 回应
```
"id"       : "request's id",    
"protocol" : "server's protocol",     
"version"  : "server's version",    
"encrypt"  : "encrypt status"     
```
如果协议或者版本不受支持，到后面disconnect部分会有说明   
一旦这三个字段检查完毕，CS连接正式建立，否则不算建立连接    
在未建立连接的情况下，发送任何其他字段的请求都会被强制断开物理连接    

## 检查延迟(ping)
### 请求
```
"id"       : "id",    
"cmd"      : "ping"    
```
目前设计不允许增添负载(payload)检测上下行网速
### 回应
```
"id"       : "request's id",    
"cmd"      : "pong"    
```

## 获取认证方式(Get authentication method)
### 请求
```
"id"       : "id",    
"cmd"      : "query",   
"target"   : "auth_method",
"params"   : {
    "opinion"      : "list_enabled(default)/currently_used",
	"replay_style" : "text(default)/definitions"
  }
```
在params中的为可选参数，留空就意味着使用缺省配置    
- opinion     
  - list_enabled   返回服务端目前全部支持的认证方式   
  - currently_used 返回当前(已经认证后)所使用的认证方式     
- replay_style      
  - text		   返回文本定义     
  - definitions    返回数字定义     

### 回应
```
"id"       : "id",    
"cmd"      : "push",   
"target"   : {[
    "plain_password"
  ]}    
``` 