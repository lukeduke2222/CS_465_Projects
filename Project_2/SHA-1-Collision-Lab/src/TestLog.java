import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class TestLog {
    public static void appendToFile(String filename, String toWrite) {
        try {
            File file = new File(filename);
            FileWriter fr = new FileWriter(file, true);
            fr.write(toWrite);
            fr.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void overrideFile(String filename, String toWrite) {
        try {
            File file = new File(filename);
            FileWriter fr = new FileWriter(file, false);
            fr.write(toWrite);
            fr.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
