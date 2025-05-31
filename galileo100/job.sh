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

OUTPUT_LIGHTNESS_DIR="$SLURM_SUBMIT_DIR/output_images/lightness"
OUTPUT_AVERAGE_DIR="$SLURM_SUBMIT_DIR/output_images/average"
OUTPUT_LUMINOSITY_DIR="$SLURM_SUBMIT_DIR/output_images/luminosity"
OUTPUT_ROOTMEANSQUARE_DIR="$SLURM_SUBMIT_DIR/output_images/rootmeansquare"
OUTPUT_REDCHANNEL_DIR="$SLURM_SUBMIT_DIR/output_images/redchannel"
OUTPUT_GREENCHANNEL_DIR="$SLURM_SUBMIT_DIR/output_images/greenchannel"
OUTPUT_BLUECHANNEL_DIR="$SLURM_SUBMIT_DIR/output_images/bluechannel"


if [ ! -d "$OUTPUT_LIGHTNESS_DIR" ]; then
  mkdir -p "$OUTPUT_LIGHTNESS_DIR"
fi
if [ ! -d "$OUTPUT_AVERAGE_DIR" ]; then
  mkdir -p "$OUTPUT_AVERAGE_DIR"
fi
if [ ! -d "$OUTPUT_LUMINOSITY_DIR" ]; then
  mkdir -p "$OUTPUT_LUMINOSITY_DIR"
fi
if [ ! -d "$OUTPUT_ROOTMEANSQUARE_DIR" ]; then
  mkdir -p "$OUTPUT_ROOTMEANSQUARE_DIR"
fi
if [ ! -d "$OUTPUT_REDCHANNEL_DIR" ]; then
  mkdir -p "$OUTPUT_REDCHANNEL_DIR"
fi
if [ ! -d "$OUTPUT_GREENCHANNEL_DIR" ]; then
  mkdir -p "$OUTPUT_GREENCHANNEL_DIR"
fi
if [ ! -d "$OUTPUT_BLUECHANNEL_DIR" ]; then
  mkdir -p "$OUTPUT_BLUECHANNEL_DIR"
fi

echo "[$(date '+%F %T')] Input directory:  $INPUT_DIR"

echo "[$(date '+%F %T')] Output directory (Lightness): $OUTPUT_LIGHTNESS_DIR"
echo "[$(date '+%F %T')] Output directory (Average): $OUTPUT_AVERAGE_DIR"
echo "[$(date '+%F %T')] Output directory (Luminosity): $OUTPUT_LUMINOSITY_DIR"
echo "[$(date '+%F %T')] Output directory (RootMeanSquare): $OUTPUT_ROOTMEANSQUARE_DIR"
echo "[$(date '+%F %T')] Output directory (RedChannel): $OUTPUT_REDCHANNEL_DIR"
echo "[$(date '+%F %T')] Output directory (GreenChannel): $OUTPUT_GREENCHANNEL_DIR"
echo "[$(date '+%F %T')] Output directory (BlueChannel): $OUTPUT_BLUECHANNEL_DIR"

echo "[$(date '+%F %T')] Container:        $SLURM_SUBMIT_DIR/image_to_grayscale.sif"

singularity exec "$SLURM_SUBMIT_DIR/image_to_grayscale.sif" \
    convert_grayscale "$INPUT_DIR" "$OUTPUT_LIGHTNESS_DIR" Lightness

singularity exec "$SLURM_SUBMIT_DIR/image_to_grayscale.sif" \
    convert_grayscale "$INPUT_DIR" "$OUTPUT_AVERAGE_DIR" Average

singularity exec "$SLURM_SUBMIT_DIR/image_to_grayscale.sif" \
    convert_grayscale "$INPUT_DIR" "$OUTPUT_LUMINOSITY_DIR" Luminosity

singularity exec "$SLURM_SUBMIT_DIR/image_to_grayscale.sif" \
    convert_grayscale "$INPUT_DIR" "$OUTPUT_ROOTMEANSQUARE_DIR" RootMeanSquare

singularity exec "$SLURM_SUBMIT_DIR/image_to_grayscale.sif" \
    convert_grayscale "$INPUT_DIR" "$OUTPUT_REDCHANNEL_DIR" RedChannel

singularity exec "$SLURM_SUBMIT_DIR/image_to_grayscale.sif" \
    convert_grayscale "$INPUT_DIR" "$OUTPUT_GREENCHANNEL_DIR" GreenChannel

singularity exec "$SLURM_SUBMIT_DIR/image_to_grayscale.sif" \
    convert_grayscale "$INPUT_DIR" "$OUTPUT_BLUECHANNEL_DIR" BlueChannel


EXIT_CODE=$?
echo "[$(date '+%F %T')] Singularity exited with code: $EXIT_CODE"
exit $EXIT_CODE