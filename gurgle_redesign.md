# LinkC Protocol   
## 简述    
LinkC 协议基于JSON（ 没有更多的说明了）    
## 启动连接(Start up connection)    
### 请求
```
"id"        : "id",    
"cmd"       : "connect",
"params"    : {
  "protocol"  : "gurgle",
  "version"   : "unusable",
  "encrypt"   : "disabled"
}
```
现在协议只支持 "gurgle"   
现在版本只存在 "unusable"   
现在加密只支持 "disabled"   
### 回应
```
"id"        : "request's id",    
"reply"     : {
  "status"  : "connection established/failed"
},
"cmd"       : "kill",
"params"    : {
  "error"   :   "short description of error",
  "reason"  :   "human-readable description of reason"
}
```
如果链接正常建立，则只存在"reply"字段中有 "status" : "connection established"
如果链接建立失败，则"reply":{"status":"connection failed"},"cmd":"kill"以及后面字段

## 检查延迟(ping)
### 请求
```
"id"        : "id",    
"cmd"       : "ping"    
```
目前设计不允许增添负载(payload)检测上下行网速
### 回应
```
"id"        : "request's id",    
"reply"     : "pong"
```

## 获取认证方式(Get authentication method)
### 请求
```
"id"        : "id",    
"cmd"       : "query",   
"params"    : {
    "target"        : "auth_method",
    "opinion"       : "list_enabled(default)",
    "replay_style"  : "text(default)"
  }
```
在params中的为可选参数，留空就意味着使用缺省配置    
- target  
  - auth_method    本次请求的为认证方式，故target为auth_method
- opinion     
  - list_enabled   返回服务端目前全部支持的认证方式   
- replay_style      
  - text		       返回文本定义     

### 回应
```
"id"        : "request's id",    
"reply"     : {
  "auth_method"   : ['plain_password']
}
``` 
回复值为一个list

## 获取认证状态(Get authentication status)
### 请求
```
"id"        : "id",
"cmd"       : "query",
"params"    : {
  "target"  : "auth_status"
}
```
### 回应
```
"id"        : "request's id",    
"reply"     : {
  "auth_status"  : "Unauthenticated/Authenticated"
}
``` 

## 获取协议版本(Get protocol's version)
### 请求
```
"id"        : "id",
"cmd"       : "query",
"params"    : {
  "target"  : "version"
}
```
### 回应
```
"id"        : "request's id",    
"reply"     : {
  "version" : "server's version"
}
``` 

## 明文认证(Plain password authentication)
### 请求:
```
"id"        : id,
"cmd"       : "auth", 
"from"      : gurgle_id, 
"params": {
  "method"  : "plain_password",
  "password": "password"
  }
}
```
### 回应:
```
"id": id, "to": full_gurgle_id, "error": null
"id": id, "to": full_gurgle_id, "error": "password", "reason": "Incorrect username or password."
"id": id, "to": full_gurgle_id, "error": "user_disabled", "reason": "Your account has been disabled or deactivated."
```

## 推送好友列表(Push roster)
### 推送
```
"id"        : 0,
"cmd"       : "push",
"params"    : {
  "target"  : "roster",
  "value"   : [
    ["user1","status","mood"],
    ["user2","status","mood"],
    ......
  ]
}
```