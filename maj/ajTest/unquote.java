package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class unquotetest {

    public static void main(String myargs[]) {

	int x = 1;
	infer crazyid = `[#(Identifier)[x+y]];

	System.out.println(crazyid.unparse());
    }
}
