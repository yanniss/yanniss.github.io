package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class itdectest {

    public static void main(String myargs[]) {

	// Inter-type Declarations
	infer idec1 =
		`[declare parents: Foo extends Bar;];

	infer idec2 =
		`[declare parents: Foo implements Bar1,Bar2;];
	infer idec3 =
		`[declare warning: set(* Point.*) && !within(Point):"bad set";];
	infer idec4 =
		`[declare error: call(Singleton.new(..)):"bad construction";];
	infer idec5 =
	    `[declare soft: IOException: execution(Foo.new(..));];
	
	infer idec6 =
	    `[declare precedence: Security, Logging;];

	System.out.println(idec1.unparse());
	System.out.println(idec2.unparse());
	System.out.println(idec3.unparse());
	System.out.println(idec4.unparse());
	System.out.println(idec5.unparse());
	System.out.println(idec6.unparse());
    }
}
