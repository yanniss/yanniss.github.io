package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class advicetest {

    public static void main(String myargs[]) {

	// Advice
	infer adv1 =
		`[before():get(int Foo.y) {}];

	infer adv2 =
		`[before(int i):get(int Foo.y) {}];
	infer adv3 =
		`[before(Object o):get(int Foo.y) {}];

	infer adv4 =
		`[after():get(int Foo.y) {}];
	infer adv5 =
		`[after(int i):get(int Foo.y) {}];

	infer adv6 =
		`[after(Object o):get(int Foo.y) {}];
	infer adv7 =
		`[after() throwing:get(int Foo.y) {}];

	infer adv8 =
		`[after() throwing(NotFoundException e):get(int Foo.y) {}];

	infer adv9 =
		`[Object around(): call(int Foo.m(int)) {}];

	// Pointcut Declaration
	infer pdec1 =
		`[private pointcut pc();];
	infer pdec2 =
	    `[pointcut pc(int i):set(int Foo.x) && args(i);];
	infer pdec4 =
	    `[abstract pointcut pc(Object o);];
	infer pdec3 =
	`[abstract pointcut pc();];

	System.out.println(adv1.unparse());
	System.out.println(adv2.unparse());
	System.out.println(adv3.unparse());
	System.out.println(adv4.unparse());
	System.out.println(adv5.unparse());
	System.out.println(adv6.unparse());
	System.out.println(adv7.unparse());
	System.out.println(adv8.unparse());
	System.out.println(adv9.unparse());
	System.out.println(pdec1.unparse());
	System.out.println(pdec2.unparse());
	System.out.println(pdec4.unparse());
	System.out.println(pdec3.unparse());

    }
}
