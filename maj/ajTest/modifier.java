package maj.ajTest;

import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class modifiertest {

    public static void main(String myargs[]) {

	// Modifierss
	infer mods1 = `[public static];
	Modifiers[] mods2 = `[public static];
	infer mods3 = `(Modifiers[])[public static];
	Modifiers[] mods4 = `(Modifiers[])[public static];
	infer odd = `[];

	infer selfmods1 = `(Modifiers[])[#mods4];
	infer selfmods2 = `(Modifiers[])[#(Modifiers[])mods4];

	System.out.println(mods1.unparse());
	System.out.println(mods2.unparse());
	System.out.println(mods3.unparse());
	System.out.println(mods4.unparse());
	System.out.println(odd.unparse());
	System.out.println(selfmods1.unparse());
	System.out.println(selfmods2.unparse());

    }
}
