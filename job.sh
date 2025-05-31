#!/bin/bash

#SBATCH --job-name=image_to_grayscale
#SBATCH --partition=g100_all_serial
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=2G
#SBATCH --time=00:10:00
#SBATCH --output=job_%j.out
#SBATCH --error=job_%j.err

module load singularity

cd "$SLURM_SUBMIT_DIR"

INPUT_DIR="$SLURM_SUBMIT_DIR/input_images"
OUTPUT_DIR="$SLURM_SUBMIT_DIR/output_images"

if [ ! -d "$OUTPUT_DIR" ]; then
  mkdir -p "$OUTPUT_DIR"
fi

echo "[$(date '+%F %T')] Input directory:  $INPUT_DIR"
echo "[$(date '+%F %T')] Output directory: $OUTPUT_DIR"
echo "[$(date '+%F %T')] Container:        $SLURM_SUBMIT_DIR/image_to_grayscale.sif"

singularity exec "$SLURM_SUBMIT_DIR/image_to_grayscale.sif" \
    image_to_grayscale "$INPUT_DIR" "$OUTPUT_DIR"

EXIT_CODE=$?
echo "[$(date '+%F %T')] Singularity exited with code: $EXIT_CODE"
exit $EXIT_CODE