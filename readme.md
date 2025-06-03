# Software Engineering for HPC - DevOps Project - Second Part

## Partecipant:
- Massimo Albino Sandretti (10764503)

## Tests

Below is a list of all implemented Google Test cases in `test_image_processing.cpp`, with a brief description of each:

- **`RedChannelConversion`**  
  Verifies that a single red pixel (`{255, 0, 0}`) is converted to 255 when using `GrayscaleMethod::RedChannel`.

- **`LuminosityConversion`**  
  Checks that a white pixel (`{255, 255, 255}`) yields 254 (due to rounding) under the luminosity formula (0.21·R + 0.72·G + 0.07·B).

- **`AverageConversion`**  
  Ensures a single pixel with RGB `(30, 90, 150)` converts to 90 via the arithmetic mean `(30 + 90 + 150) / 3`.

- **`EmptyImage`**  
  Confirms that passing an empty image (0×0) produces an empty `result` vector.

- **`ImageSize`**  
  Verifies that a 1×1 input image yields a 1×1 output (structure check).

- **`ImageSize2`**  
  Verifies that a 1×2 input image yields a 1×2 output (structure check).

- **`ImageSize3`**  
  Verifies that a 2×1 input image yields a 2×1 output (structure check).

- **`ImageSize4`**  
  Verifies that a 4×2 input image yields a 4×2 output (structure check).

- **`RedChannel`**  
  For a 4×2 test image with varied RGB values, checks that each output pixel equals its corresponding red-channel value.

- **`GreenChannel`**  
  For the same 4×2 test image, checks that each output pixel equals its corresponding green-channel value.

- **`BlueChannel`**  
  For the same 4×2 test image, checks that each output pixel equals its corresponding blue-channel value.

- **`Lightness`**  
  Uses a 3×2 test image to verify the “lightness” formula `((max(R, G, B) + min(R, G, B)) / 2)` for each pixel.

- **`Average`**  
  Uses a 3×2 test image to validate the arithmetic mean `(R + G + B) / 3` for each pixel.

- **`RootMeanSquare`**  
  Uses a 3×2 test image to validate `sqrt((R² + G² + B²) / 3)` for each pixel.

- **`BlackPixel`**  
  Converts a single black pixel (`{0, 0, 0}`) under **all methods** (Red, Green, Blue, Lightness, Average, Luminosity, RootMeanSquare) and verifies the result is always 0.

- **`WhitePixel`**  
  Converts a single white pixel (`{255, 255, 255}`) under **all methods** and verifies the result is always 255.

- **`MultiplePixelsAndMethods`**  
  Uses a 2×2 image with red, green, blue, and yellow pixels.  
  - Checks each channel method (Red, Green, Blue) produces correct per-channel outputs.  
  - Checks `Lightness`, `Average`, and `RootMeanSquare` produce the correct values for all four pixels.  

##  CI/CD Pipeline Overview

As requested I used Github Actions to automate building, testing, containerizing, and deploying to the CINECA cluster. The workflow consists of three main jobs:

---

### 1. build_and_test  
**Runs on**: ubuntu-latest  
**Purpose**: Compile the C/C++ code and run all unit tests to ensure code compiles cleanly and all grayscale-conversion tests pass before proceeding.

1. **Checkout & Submodules**  
   - Uses `actions/checkout@v4` (with submodules) to grab code and any nested repositories.  
2. **Configure Build**  
   - Initializes submodules and creates a `build/` directory.  
   - Invokes `cmake ..` to generate makefiles.  
3. **Compile**  
   - Runs `make` inside `build/`.  
4. **Execute Tests**  
   - Invokes `ctest --output-on-failure` to run Google Test cases, failing the job if any test fails.  

---

### 2. build_container  
**Needs**: `build_and_test`  
**Runs on**: ubuntu-latest  
**Purpose**: Package the application into a Singularity container to creates a portable container ready for HPC cluster deployment.

1. **Checkout with Submodules**  
   - Retrieves repo and `singularity.def`.  
2. **Install Singularity**  
   - Installs singularity-container`.  
3. **Build Container Image**  
   ```bash
   sudo singularity build image_to_grayscale.sif singularity.def
   ```
4. **Upload Artifact**  
   - Saves `image_to_grayscale.sif` via `actions/upload-artifact@v4`.  

---

### 3. send_to_cluster  
**Needs**: `build_container`  
**Runs on**: ubuntu-latest  
**Purpose**: Transfer files, run job via SLURM, and fetch output.

1. **Checkout**  
   - Grabs SLURM scripts and inputs.  
2. **Download Container**  
   - Retrieves `image_to_grayscale.sif` artifact.  
3. **Install `expect`**  
   ```bash
   sudo apt-get update && sudo apt-get install -y expect
   ```
4. **Prepare SSH Keys**  
   - Decodes secrets and sets up `ssh-agent`.  
5. **Load Private Key**  
   ```bash
   expect <<EOF
     spawn ssh-add "$HOME/.ssh/my_key"
     expect "Enter passphrase for $HOME/.ssh/my_key:"
     send "$SSH_PASSPHRASE"
     expect eof
   EOF
   ```
6. **Create Remote Directory**  
   ```bash
   ssh -i ~/.ssh/my_key -o StrictHostKeyChecking=no $CINECA_USERNAME@login.g100.cineca.it      "rm -rf ~/jobs/img_to_grayscale_automatic && mkdir -p ~/jobs/img_to_grayscale_automatic"
   ```
7. **Upload Files**  
   ```bash
   ssh-keygen -f ~/.ssh/known_hosts -R login.g100.cineca.it
   scp -i ~/.ssh/my_key -o StrictHostKeyChecking=no      galileo100/job.sh $CINECA_USERNAME@login.g100.cineca.it:~/jobs/img_to_grayscale_automatic/
   ```
   - Also uploads `input_images/` and `image_to_grayscale.sif`.  
8. **Submit and Monitor SLURM Job**  
   ```bash
   jobid=$(ssh -i ~/.ssh/my_key -o StrictHostKeyChecking=no      $CINECA_USERNAME@login.g100.cineca.it "cd ~/jobs/img_to_grayscale_automatic/ && sbatch job.sh" | awk '{print $4}')
   echo "Job ID: $jobid"

   while ssh -i ~/.ssh/my_key -o StrictHostKeyChecking=no          $CINECA_USERNAME@login.g100.cineca.it "squeue -j $jobid" | grep -q $jobid; do
     echo "Job $jobid is still running..."
     sleep 5
   done
   echo "Job $jobid completed."
   ```
9. **Retrieve Outputs**  
   ```bash
   ssh -i ~/.ssh/my_key -o StrictHostKeyChecking=no      $CINECA_USERNAME@login.g100.cineca.it "cat ~/jobs/img_to_grayscale_automatic/job_${jobid}.out"
   ssh -i ~/.ssh/my_key -o StrictHostKeyChecking=no      $CINECA_USERNAME@login.g100.cineca.it "cat ~/jobs/img_to_grayscale_automatic/job_${jobid}.err"

   scp -i ~/.ssh/my_key -o StrictHostKeyChecking=no      -r $CINECA_USERNAME@login.g100.cineca.it:~/jobs/img_to_grayscale_automatic/output_images ./output_images
   ```
10. **Upload Output**  
    ```yaml
    uses: actions/upload-artifact@v4
    with:
      name: output-images
      path: ./output_images
    ```



## Problems & Solutions
### Connection to the Galileo Cluster
the main problem encountered was connecting to the Galileo cluster bypassing the two factor authentication.
The solution was found reading through the Cineca User Guide, which explained how to create a certificate and key to use as credentials. Those credential have a validity of 12 hours. The steps I followed are awailable [here](https://docs.hpc.cineca.it/general/access.html).
In order to use these credentials in github action, I created ifve secrets:
- `SSH_CERT`: the certificate in base64 format
- `SSH_PRIVATE_KEY`: the private key in base64 format
- `SSH_PUBLIC_KEY`: the public key in base64 format   
- `SSH_PASSPHRASE`: the key's passphrase 
- `CINECA_USERNAME`: my username on the Galileo cluster

Looking through the actions history, it is possible to see that the connection to the cluster is successful, and the results of the computation are awailable for download as artifacts (both the singularity image and the output images produced by running the job on the cluster).

