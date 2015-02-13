# Gurgle basic package
## Package Header
<pre><code>
"Protocol"  : "Gurgle",
"Version"   : "Unusable",
"Action"    : "Connect/Get/Post/Login/Logout",
"Target"    : "Default/Gurgle ID/Chat Message",
"Extend"    : "Extend data"
</code></pre>
### Action
* __Get__   : Request the resources that the __Target__ specified.  
* __Post__  : Upload.............  
* __Login__ : Login, The __Target__ must be "Default"  
* __Logout__: Logout ............
* __Connect__: If the __Target__ is "Default", Then check protocol's version. if the __Target__ is one's ID,then help them to create P2P connection.
## Extend Body
### Gurgle ID
<pre><code>
"Key"       : "GurgleID",
"Type"      : "Personal/Group"
"Username"  : "UserName/GroupName",
"Domain"    : "Domain/Default",
"Terminal"  : "Terminal/Default".
</code></pre>

### User Data
<pre><code>
"Key"       : "UserData",
"GurgleID"  : "Gurgle ID",
"Status"    : "Online/Offline/Invisible/Away",
"Parvate"   : "Parvate Data/Default"
</code></pre>

### Message
<pre><code>
"Key"       : "ChatMessage",
"DestID"    : "Gurgle ID",
"FromID"    : "Gurgle ID",
"Date"      : "Send Time",
"Message"   : "Message"
</code></pre>
