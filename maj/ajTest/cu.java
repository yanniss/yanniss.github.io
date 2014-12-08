package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class cutest {

    public static void main(String myargs[]) {

	// Compilation Unit
	infer cu1 =
		`[package blah.baz;
		  import import1.*;
			class foo {}
		];

	infer cu2 =
	    `[import java.lang.*;
	      
	      class foo {}
	    ];

	infer cu3 =	//Hmmm, where is foo2???
	    `[import java.lang.*;
		class foo1 {}
		class foo2 {}
		];

	System.out.println(cu1.unparse());
	System.out.println(cu2.unparse());
	System.out.println(cu3.unparse());
    }
}
