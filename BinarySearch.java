import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;
import java.util.ArrayList;

public class BinarySearch {

    public static int binarySearch(ArrayList<String[]> array, String target) {
        int left = 0;
        int right = array.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

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
        long totalTime = 0;
        
        long startTime = System.nanoTime();
        for (int i = 0; i < iterations; i++) {
            binarySearch(array, target);
        }
        long endTime = System.nanoTime();
        totalTime += (endTime - startTime);
    
        return totalTime;
    }

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java BinarySearch <input_csv_file>");
            return;
        }
        
        String inputFileName = args[0];
        ArrayList<String[]> dataList = new ArrayList<>();

        try {
            File myObj = new File(inputFileName);
            Scanner myReader = new Scanner(myObj);
            while (myReader.hasNextLine()) {
                String[] dataArray = myReader.nextLine().split(",");
                dataList.add(dataArray);
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred: File not found - " + inputFileName);
            e.printStackTrace();
            return;
        }

        int n = dataList.size();

        for (int i = 0; i < 100000; i++) {
            binarySearch(dataList, dataList.get(i % n)[0]);
        }
        
        // Calculate average case using multiple elements
        int[] positions = {n/8, n/6, n/4, n/3, (2*n)/5, (3*n)/5, (2*n)/3, (3*n)/4};
        long totalAverageCaseTime = 0;
        for (int pos : positions) {
            String element = dataList.get(pos)[0];
            totalAverageCaseTime += measureSearchTime(dataList, element, n);
        }
        long averageCaseTime = totalAverageCaseTime / positions.length;
        
        int last = n - 1;
        String lastElement = dataList.get(last)[0];
        long worstCaseTime = measureSearchTime(dataList, lastElement, n);
        
        int mid = n / 2;
        String middleElement = dataList.get(mid)[0];
        long bestCaseTime = measureSearchTime(dataList, middleElement, n);

        StringBuilder output = new StringBuilder();
        output.append("Binary Search Performance Analysis\n\n");
        output.append(String.format("Number of searches performed for each case: %d\n\n", n));
        output.append(String.format("Best case (middle element) average time: %.6f milliseconds\n", bestCaseTime / 1_000_000.0));
        output.append(String.format("Average case (average of elements at 1/8, 1/6, 1/4, 1/3, 2/5, 3/5, 2/3, and 3/4 positions) average time: %.6f milliseconds\n",
                averageCaseTime / 1_000_000.0));
        output.append(String.format("Worst case (last element) average time: %.6f milliseconds\n", worstCaseTime / 1_000_000.0));

        String outputFileName = "binary_search_" + n + ".txt";
        try {
            FileWriter writer = new FileWriter(outputFileName);
            writer.write(output.toString());
            writer.close();
            System.out.println("Results have been written to " + outputFileName);
        } catch (IOException e) {
            System.out.println("An error occurred while writing to the file.");
            e.printStackTrace();
        }
    }
}