// Adder.java
public class Adder {
    // Load the native library
   static {
    System.load("C:\\Users\\harine-pt7602\\source\\repos\\Adder\\Adder\\adder.dll");
}

    // Native method declaration
    public native int add(int a, int b);

    // Main method to test the native method
    public static void main(String[] args) {
        Adder adder = new Adder();
        int result = adder.add(5, 3);
        System.out.println("Result: " + result);
    }
}
