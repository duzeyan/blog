## 使用工具*postman* + *edit this Cookies* 协助工作

###一、使用动机
  - 避免测试的时候重复填表单，提高工作效率
  - 检验其他网站安全性


###二、工具介绍
 [postman](http://chromecj.com/web-development/2014-09/60.html)是一款Chrome的扩展插件，可以帮助用户填写http报文并选定方式发送。对于我们web开发人员很有帮助。同样
 [editthisCookies](http://www.chromein.com/crx_11241.html)也是Chrome一款小巧实用的工具。可以用来修改cookies，Chrome本身也是有cookies管理的，但是只能查看和删除，相对来说功能就比较简单。下面来介绍一个应用场景，来介绍我们是如何来使用这两个工具提高我们的工作效率。详细用法见链接。

###三、应用场景
这里用J2EE的servlet作为后台处理程序，其他asp,php等类似。后台的逻辑是前台传送的数据进行打印。并用cookies记录访问次数。<BR/>
**GET请求处理方式**

	public void doGet(HttpServletRequest request, HttpServletResponse response)
				throws ServletException, IOException {
			System.out.println("使用GET方式，下面转到doPost处理");
	        doPost(request, response);
		}

**POST请求处理方式**

	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		String time=null;     //用来计算请求次数 存在cookies
		
		Cookie[] cookies = request.getCookies();	
		if(cookies!=null){
			for(Cookie cookie : cookies){//提取cookies
			    if(cookie.getName().equals("time")){
			    	time=cookie.getValue(); 
			    	break;
			    }
			}
		}
		if(time==null){ //cookies失效或者第一次请求
			time="1";
		}else{ //请求次数加一
			time=Integer.parseInt(time)+1+"";
		}
		
	  //添加(更新)到cookies
	   Cookie cookie = new Cookie("time",time);
       cookie.setMaxAge(3600);	    	    
	   cookie.setPath("/");
	   response.addCookie(cookie);

	   //打印数据
	   String u=request.getParameter("username");
	   String p=request.getParameter("password");
	   System.out.println("用户名:"+u);
	   System.out.println("密码:"+p);
	   System.out.println("访问次数"+time);
	   
	   response.sendRedirect("MyJsp.jsp");
	}

**前台表单**
	
	 <form action="UserLogin" method="post">
	         账号：  <input type="text"  name="username" /><br/>
	         密码： <input type="password" name="password" /><br/>
	                <input type="submit" value="登录">
	  </form>

以上构架了一个基本的http请求。其中cookies的操作我们放在服务器中操作，用来记录用户登录请求的次数。


 页面输入数据，普通的一次请求结果如下：

  <center>![](http://dl.iteye.com/upload/picture/pic/133083/bf9274d3-505b-3a52-bafe-d182a33a15ed-thumb.png)</center>
这是很常规的一次请求，假设我们的表单数据很多，每次调试都需要填写表单很费事费力。这个时候postman就派上用场了。打开postman

如图。
  <center>![](http://dl.iteye.com/upload/picture/pic/133089/e43304ae-d567-3720-8621-9ff4b271b33a-thumb.png)</center>
红色为历史记录。黄色为填数据的地方。蓝色部分可以选择请求方式，比如数据POST,GET,PUT... 本例中使用get方式。绿色部分为我们的请求地址，这里是登陆的servlet的地址。最后黑色部分显示返回的结果和状态码，图中为200，表示访问成功。这次请求后台输出结果如下：
<center>![](http://dl.iteye.com/upload/picture/pic/133085/739b569c-7fce-3a0b-b286-900ad9528189-thumb.png)</center>

有时候我们不仅想操作表单，还想操作cookies，比如这个例子里设定当time大于100的时候给用户1个积分，为了测试这个功能，总不能手动登陆100次，但是如果直接代码直接设置又比较麻烦，因为测试完还要改回去，这个时候我们可以用上面介绍的edit this cookies扩展工具。界面如下：


![](http://dl.iteye.com/upload/picture/pic/133087/be0a589c-e5a1-3cc6-bae0-b3b2beae4c6b-thumb.png)
 如图我直接把time的值从1修改为5，提交一次请求后输出如下变为6（本来应该输出2）：
<center>![](http://dl.iteye.com/upload/picture/pic/133079/b0c2c743-4a27-3cc6-8553-d1917218712d-thumb.png)</center>



###四、总结

 虽然是两个小工具，但是在实际开发中特别是调试的时候使用，会极大提高效率。如果以后赚钱了就给这两个扩展的设计者捐助一下吧O(∩_∩)O~


