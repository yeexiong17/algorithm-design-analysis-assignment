import java.io.*;
import java.util.*;

public class QuickSort {

    // Read CSV data into a list
    public static List<String[]> readCSV(String filename) throws IOException {
        List<String[]> data = new ArrayList<>();
        BufferedReader br = new BufferedReader(new FileReader(filename));
        String line;
        while ((line = br.readLine()) != null) {
            String[] row = line.split(",", 2); // split only into two parts
            if (row.length >= 2) {
                data.add(row);
            }
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

    // Main function with command-line args
    public static void main(String[] args) {
        if (args.length < 1 || args.length > 2) {
            System.err.println("Usage: java QuickSort <input_file> [output_file]");
            System.err.println("If output_file is not provided, the output will be written to");
            System.err.println("a file named <input_base>_sorted.csv in the same directory.");
            System.exit(1);
        }

        String inputFile = args[0];
        String outputFile;

        if (args.length == 2) {
            outputFile = args[1];
        } else {
            File inFile = new File(inputFile);
            String baseName = inFile.getName();
            String nameWithoutExtension;

            int dotIndex = baseName.lastIndexOf('.');
            if (dotIndex > 0) {
                nameWithoutExtension = baseName.substring(0, dotIndex);
            } else {
                nameWithoutExtension = baseName;
            }

            outputFile = new File(inFile.getParent(), nameWithoutExtension + "_sorted.csv").toString();
        }

        try {
            List<String[]> data = readCSV(inputFile);

            int startRow = 0;
            int endRow = data.size() - 1;

            // Generate log file name dynamically
            String stepOutputFile = "quick_sort_step_" + startRow + "_" + endRow + ".txt";
            BufferedWriter writer = new BufferedWriter(new FileWriter(stepOutputFile));

            // Write initial array
            writer.write(formatSubarray(data, startRow, endRow));
            writer.newLine();

            // Start sorting with step tracking
            int[] stepCounter = {1};
            quickSortWithTrace(data, startRow, endRow, writer, stepCounter);
            writer.close();

            // Save final sorted output
            writeSortedData(data, outputFile);

            // Measure time (excluding I/O)
            long startTime = System.nanoTime();
            quickSort(data, 0, data.size() - 1); // Final sort (not logged)
            long endTime = System.nanoTime();

            double durationSeconds = (endTime - startTime) / 1_000_000_000.0;
            System.out.printf("Quick Sort completed in %.6f seconds%n", durationSeconds);

        } catch (IOException e) {
            System.err.println("Error processing files: " + e.getMessage());
            e.printStackTrace();
        } catch (NumberFormatException e) {
            System.err.println("Invalid numeric value in CSV file: " + e.getMessage());
        }
    }
}
