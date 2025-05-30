import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.ArrayList;

public class BinarySearch {

    public static int binarySearch(ArrayList<String[]> array, String target) {
        int left = 0;
        int right = array.size() - 1;

        while (left <= right) {
            int mid = (right + left) / 2;

            if (array.get(mid)[0].equals(target)) {
                return mid;
            } else if (array.get(mid)[0].compareTo(target) < 0) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        return -1;
    }

    public static long measureSearchTime(ArrayList<String[]> array, String target, int iterations) {
        long startTime = System.nanoTime();
        for (int i = 0; i < iterations; i++) {
            binarySearch(array, target);
        }
        long endTime = System.nanoTime();
        return (endTime - startTime) / iterations;
    }

    public static void main(String[] args) {
        // Use ArrayList for dynamic sizing
        ArrayList<String[]> dataList = new ArrayList<>();

        try {
            File myObj = new File("dataset_sample_1000.csv");
            Scanner myReader = new Scanner(myObj);
            while (myReader.hasNextLine()) {
                String[] dataArray = myReader.nextLine().split(",");
                dataList.add(dataArray);
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
            return;
        }

        int n = dataList.size();

        int mid = n / 2;
        String middleElement = dataList.get(mid)[0];
        long bestCaseTime = measureSearchTime(dataList, middleElement, n);

        int quarter = n / 4;
        String quarterElement = dataList.get(quarter)[0];
        long averageCaseTime = measureSearchTime(dataList, quarterElement, n);

        int last = n - 1;
        String lastElement = dataList.get(last)[0];
        long worstCaseTime = measureSearchTime(dataList, lastElement, n);

        System.out.printf("Binary Search Performance Analysis%n%n", n);
        System.out.printf("Number of searches performed for each case: %d%n%n", n);
        System.out.printf("Best case (middle element) time: %.6f seconds%n", bestCaseTime / 1_000_000_000.0);
        System.out.printf("Average case (other element than first, last and middle) time: %.6f seconds%n",
                averageCaseTime / 1_000_000_000.0);
        System.out.printf("Worst case (last element) time: %.6f seconds%n", worstCaseTime / 1_000_000_000.0);
    }
}