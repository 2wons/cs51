import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.*;

public class ClientSimple {

    private static PrintWriter out ;
    private static BufferedReader in;

    public static void main(String[] args) throws IOException {

        Socket s = new Socket("127.0.0.1",8080);

        out = new PrintWriter(s.getOutputStream(),true);
        out.println("Hello from Java Program....");

        in = new BufferedReader(new InputStreamReader(s.getInputStream()));
        System.out.println(in.readLine());
    }
}