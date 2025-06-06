import java.io.*;
import java.util.*;

public class QuickSort {

    // Read CSV data into a list
    public static List<String[]> readCSV(String filename) throws IOException {
        List<String[]> data = new ArrayList<>();
        BufferedReader br = new BufferedReader(new FileReader(filename));
        String line;
        while ((line = br.readLine()) != null) {
            data.add(line.split(","));
        }
        br.close();
        return data;
    }

    // Write sorted data to output file
    public static void writeSortedData(List<String[]> data, String filename) throws IOException {
        BufferedWriter bw = new BufferedWriter(new FileWriter(filename));
        for (String[] row : data) {
            bw.write(row[0] + "," + row[1]);
            bw.newLine();
        }
        bw.close();
    }

    // Utility method to print subarray in desired format
    private static String formatSubarray(List<String[]> data, int low, int high) {
        StringBuilder sb = new StringBuilder("[");
        for (int i = low; i <= high; i++) {
            sb.append(data.get(i)[0]).append("/").append(data.get(i)[1]);
            if (i != high) sb.append(", ");
        }
        sb.append("]");
        return sb.toString();
    }

    // Partition function with logging
    public static int partition(List<String[]> data, int low, int high, BufferedWriter writer, int[] stepCounter) throws IOException {
        long pivot = Long.parseLong(data.get(high)[0]);
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (Long.parseLong(data.get(j)[0]) <= pivot) {
                i++;
                Collections.swap(data, i, j);
            }
        }
        Collections.swap(data, i + 1, high);

        // Log current state after partitioning
        String logLine = "pi=" + stepCounter[0] + " " + formatSubarray(data, low, high);
        writer.write(logLine);
        writer.newLine();
        stepCounter[0]++;

        return i + 1;
    }

    // Recursive quickSort with logging
    public static void quickSortWithTrace(List<String[]> data, int low, int high, BufferedWriter writer, int[] stepCounter) throws IOException {
        if (low < high) {
            int pi = partition(data, low, high, writer, stepCounter);
            quickSortWithTrace(data, low, pi - 1, writer, stepCounter);
            quickSortWithTrace(data, pi + 1, high, writer, stepCounter);
        }
    }

    // Main function
    public static void main(String[] args) throws Exception {
        String input = "dataset_sample_1000.csv";
        String output = "quick_sort_1000.csv";

        // Output file for step-by-step trace
        int startRow = 0;
        int endRow = 999;  // Adjust based on dataset size
        String stepOutputFile = "quick_sort_step_" + startRow + "_" + endRow + ".txt";

        List<String[]> data = readCSV(input);

        // Optional: Shuffle before sorting to ensure unsorted input
        Collections.shuffle(data);

        // Open writer for step tracing
        BufferedWriter writer = new BufferedWriter(new FileWriter(stepOutputFile));

        // Print initial array
        writer.write(formatSubarray(data, startRow, endRow));
        writer.newLine();

        // Start sorting with step tracking
        int[] stepCounter = {1};
        quickSortWithTrace(data, startRow, endRow, writer, stepCounter);

        writer.close();

        // Save final sorted output
        writeSortedData(data, output);

        // Measure time (excluding I/O)
        long startTime = System.nanoTime();
        quickSort(data, 0, data.size() - 1);  // Final sort (not logged)
        long endTime = System.nanoTime();

        double durationSeconds = (endTime - startTime) / 1_000_000_000.0;
        System.out.println("Quick Sort completed in " + durationSeconds + " seconds");
    }

    // Standard Quick Sort implementation
    public static void quickSort(List<String[]> data, int low, int high) {
        if (low < high) {
            int pi = partition(data, low, high);
            quickSort(data, low, pi - 1);
            quickSort(data, pi + 1, high);
        }
    }

    public static int partition(List<String[]> data, int low, int high) {
        long pivot = Long.parseLong(data.get(high)[0]);
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (Long.parseLong(data.get(j)[0]) <= pivot) {
                i++;
                Collections.swap(data, i, j);
            }
        }
        Collections.swap(data, i + 1, high);
        return i + 1;
    }
}
