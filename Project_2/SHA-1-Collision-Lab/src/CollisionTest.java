import java.util.HashSet;
import java.util.Set;

public class CollisionTest {
    Set<Integer> allHashes = new HashSet<>();
    int numHashes;
    SHAwrap encoder;

    public int run(int numBits, int rounds, int roundNum) {
        reset();
        encoder = new SHAwrap();
        int collision = loopHashes(numBits);

        String filename = "CollisionTest-"+numBits+"Bit-"+rounds+"Rounds.txt";
        String data = String.format("Trial #%02d:\t%dth hash\n", roundNum, numHashes);
        TestLog.appendToFile(filename, data);

        return numHashes;
    }

    public int loopHashes(int numBits) {
        while (true) {
            byte[] toHash = encoder.generateRandBytes(20);
            int hash = encoder.hash(toHash, numBits);
            boolean newHash = allHashes.add(hash);
            numHashes++;
            if (!newHash) return hash;
        }
    }

    public void reset() {
        allHashes = new HashSet<>();
        numHashes = 0;
    }
}
