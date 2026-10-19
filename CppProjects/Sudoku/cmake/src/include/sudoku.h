#ifndef SUDOKU_H
#define SUDOKU_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include "include/digitrecognizer.h"

using namespace cv;
using namespace std;

extern const int sizex;
extern const int sizey;

/**
 * @brief drawLine
 * @param line
 * @param img
 * @param rgb
 */
void drawLine(Vec2f line, Mat &img, Scalar rgb);

/**
 * @brief mergeRelatedLines
 * @param lines
 * @param img
 */
void mergeRelatedLines(vector<Vec2f> *lines, Mat &img);

/**
 * @brief Detects the Sudoku board, warps it to a top-down square, and optionally returns the inverse perspective transform.
 *        Returns maxLength (> 0) if a board is found, or 0 if no board is detected.
 */
int getBox(Mat& image, Mat& box, Mat& Originalbox, Mat* outInvTransform = nullptr,
           Point2f* ioSmoothedQuad = nullptr, bool* ioHasPrevQuad = nullptr,
           bool allowHoughFallback = true, Mat* outColorWarped = nullptr);

/**
 * @brief Extracts and classifies the 9x9 cells from the warped board images. Returns the number of detected non-empty digits.
 */
int extractSudokuDigits(const Mat& undistorted, const Mat& ohneGaus, int maxLength,
                        DigitRecognizer& dr, Mat& sudokuMat, Mat& altSudokuMat,
                        Mat& confidenceMat, bool showRoiGui = false);

/**
 * @brief Solves a 9x9 Sudoku board (CV_8UC1) in-place using MRV backtracking.
 */
bool solveSudoku(Mat& board);

/**
 * @brief Solves the Sudoku board, resolving any single-cell OCR conflicts using runner-up KNN candidates if needed.
 */
bool resolveAndSolveSudoku(Mat& board, const Mat& altBoard, const Mat& confidenceMat);

/**
 * @brief Renders the solved numbers directly onto the original image in their exact perspective position.
 */
void overlaySolvedNumbers(Mat& image, const Mat& originalBoard, const Mat& solvedBoard, int maxLength, const Mat& invTransform);

// Colores Institucionales E2i (Manual Identidad 2026 - E2i.pdf) en formato BGR de OpenCV
// Primarios (Degradado Primario - Pag. 18):
//   Nodo A - Pantone 432C  : RGB(36, 75, 90)    | HTML #244B5A -> BGR(90, 75, 36)
//   Nodo B - Pantone 0921C : RGB(153, 230, 216) | HTML #99E6D8 -> BGR(216, 230, 153)
// Secundarios (Pag. 19):
//   Pantone 432 C          : RGB(53, 62, 71)    | HTML #353E47 -> BGR(71, 62, 53)
//   Pantone 429 C          : RGB(163, 169, 173) | HTML #A3A9AD -> BGR(173, 169, 163)
//   Pantone Green 0921 C   : RGB(172, 228, 216) | HTML #ACE4D8 -> BGR(216, 228, 172)
//   Pantone Cool Gray 6 C  : RGB(227, 228, 228) | HTML #E3E4E4 -> BGR(228, 228, 227)
static const Scalar E2I_PRIMARY_DARK(90, 75, 36);       // #244B5A
static const Scalar E2I_PRIMARY_MINT(216, 230, 153);    // #99E6D8
static const Scalar E2I_SEC_SLATE(71, 62, 53);          // #353E47
static const Scalar E2I_SEC_GRAY(173, 169, 163);        // #A3A9AD
static const Scalar E2I_SEC_MINT_LIGHT(216, 228, 172);  // #ACE4D8
static const Scalar E2I_SEC_LIGHT_GRAY(228, 228, 227);  // #E3E4E4

/**
 * @brief Creates a rectified cropped image of the Sudoku board alone with the solved numbers rendered in place.
 */
Mat createSolvedBoardCrop(const Mat& colorWarpedBoard, const Mat& originalBoard, const Mat& solvedBoard);

/**
 * @brief Places the cropped solved Sudoku in the top-right corner of the frame and optionally draws connecting callout lines
 *        from the recognized board corners on the live image to the top-right box.
 */
void drawSolvedCropPiP(Mat& frame, const Mat& solvedCrop, const Point2f liveQuad[4], int topOffset = 46, bool drawCalloutLines = true);

#endif // SUDOKU_H




