public class Main {
    private final static int ROUNDS = 75;
    private final static int ROUNDS_LARGE = 5;


    public static void main(String[] args) {

        PreImageTest preImageTest = new PreImageTest();
        CollisionTest collisionTest = new CollisionTest();

        int[] bitSizes = {6, 8, 10, 12, 14, 16};
        for (int numBits : bitSizes) {

            String filename = "PreImageTest-"+numBits+"Bit-"+ROUNDS+"Rounds.txt";
            String data = String.format("Beginning %d Round PreImage Test\n", ROUNDS);
            TestLog.overrideFile(filename, data);

            double hashSum = 0;
            for(int i = 0; i < ROUNDS; i++) {
                hashSum += preImageTest.run(numBits, ROUNDS, i+1);
            }

            int expectedNumHashes = (int)Math.pow(2, numBits);
            data = String.format("\nExpected avg number of hashes: %d\nActual avg number of hashes: %.2f",
                    expectedNumHashes, (hashSum/ROUNDS));
            TestLog.appendToFile(filename, data);

            ////---------------------------------------------------////

            filename = "CollisionTest-"+numBits+"Bit-"+ROUNDS+"Rounds.txt";
            data = String.format("Beginning %d Round Collision Test\n", ROUNDS);
            TestLog.overrideFile(filename, data);

            hashSum = 0;
            for(int i = 0; i < ROUNDS; i++) {
                hashSum += collisionTest.run(numBits, ROUNDS, i+1);
            }

            expectedNumHashes = (int)Math.pow(2, ((double) numBits/2));
            data = String.format("\nExpected avg number of hashes: %d\nActual avg number of hashes: %.2f",
                    expectedNumHashes, (hashSum/ROUNDS));
            TestLog.appendToFile(filename, data);
        }


        bitSizes = new int[]{18,20,24};
        for (int numBits : bitSizes) {
            System.out.println("Beginning on bitSize "+numBits);
//
            String filename = "PreImageTest-"+numBits+"Bit-"+ROUNDS_LARGE+"Rounds.txt";
            String data = String.format("Beginning %d Round PreImage Test\n", ROUNDS_LARGE);
            TestLog.overrideFile(filename, data);

            double hashSum = 0;
            for(int i = 0; i < ROUNDS_LARGE; i++) {
                System.out.println("Beginning round "+(i+1)+" on PreImaging");
                hashSum += preImageTest.run(numBits, ROUNDS_LARGE, i+1);
            }

            int expectedNumHashes = (int)Math.pow(2, numBits);
            data = String.format("\nExpected avg number of hashes: %d\nActual avg number of hashes: %.2f",
                    expectedNumHashes, (hashSum/ROUNDS_LARGE));
            TestLog.appendToFile(filename, data);

            ////--------------------------------------------------////

            filename = "CollisionTest-"+numBits+"Bit-"+ROUNDS_LARGE+"Rounds.txt";
            data = String.format("Beginning %d Round Collision Test\n", ROUNDS_LARGE);
            TestLog.overrideFile(filename, data);

            hashSum = 0;
            for(int i = 0; i < ROUNDS_LARGE; i++) {
                hashSum += collisionTest.run(numBits, ROUNDS_LARGE, i+1);
            }

            expectedNumHashes = (int)Math.pow(2, ((double) numBits/2));
            data = String.format("\nExpected avg number of hashes: %d\nActual avg number of hashes: %.2f",
                    expectedNumHashes, (hashSum/ROUNDS_LARGE));
            TestLog.appendToFile(filename, data);
        }
    }
}
