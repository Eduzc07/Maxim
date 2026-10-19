#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
#include "include/sudoku.h"

static void drawStatusBanner(Mat& frame, const string& text, const Scalar& badgeColor) {
    int height = 38;
    if (frame.rows < height) return;
    Mat roi = frame(Rect(0, 0, frame.cols, height));
    Mat banner(roi.size(), CV_8UC3, E2I_SEC_SLATE);
    addWeighted(banner, 0.85, roi, 0.15, 0.0, roi);
    line(frame, Point(0, height - 1), Point(frame.cols, height - 1), E2I_PRIMARY_MINT, 2, LINE_AA);

    circle(frame, Point(20, height / 2), 7, E2I_PRIMARY_DARK, FILLED, LINE_AA);
    circle(frame, Point(20, height / 2), 5, badgeColor, FILLED, LINE_AA);
    putText(frame, text, Point(36, 25), FONT_HERSHEY_DUPLEX, 0.55, E2I_SEC_LIGHT_GRAY, 1, LINE_AA);
}

static bool boardsAreCompatible(const Mat& currentDetected, const Mat& cachedSolved) {
    int checked = 0;
    int matches = 0;
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            uchar d = currentDetected.at<uchar>(r, c);
            if (d >= 1 && d <= 9) {
                checked++;
                if (d == cachedSolved.at<uchar>(r, c)) {
                    matches++;
                }
            }
        }
    }
    if (checked < 8) return true; // Too blurry to invalidate cache
    return (matches >= (int)(checked * 0.65));
}

static int runRealtimeStream(VideoCapture* cap, const Mat* simBaseImage, const string& outputPath,
                             DigitRecognizer& dr, bool showGui)
{
    Point2f smoothedQuad[4];
    bool hasPrevQuad = false;

    bool hasSolvedCache = false;
    Mat cachedOrigBoard = Mat::zeros(Size(9, 9), CV_8UC1);
    Mat cachedSolvedBoard = Mat::zeros(Size(9, 9), CV_8UC1);
    Mat cachedSolvedCrop;

    int voteBuffer[9][9][10];
    std::memset(voteBuffer, 0, sizeof(voteBuffer));

    int framesWithoutBoard = 0;
    int frameCounter = 0;
    int lastCluesCount = 0;
    int maxSimFrames = simBaseImage ? 15 : 0;

    if (showGui) {
        cout << "\n=== Modo Camara en Tiempo Real (Sudoku AR - E2i) ===\n"
             << "Controles en la ventana de video:\n"
             << "  [Q / ESC] Salir\n"
             << "  [R]       Reiniciar / Re-escanear Sudoku\n"
             << "  [S]       Guardar captura en " << outputPath << "\n\n";
    }

    Mat frame;
    while (true) {
        if (cap) {
            if (!cap->read(frame) || frame.empty()) {
                cout << "Fin del stream de camara o error de lectura.\n";
                break;
            }
        } else if (simBaseImage) {
            if (frameCounter >= maxSimFrames) break;
            frame = simBaseImage->clone();
        } else {
            break;
        }

        frameCounter++;
        Mat undistorted, ohneGaus, invTransform, colorWarped;
        int maxLength = getBox(frame, undistorted, ohneGaus, &invTransform,
                               smoothedQuad, &hasPrevQuad, /*allowHoughFallback=*/false, &colorWarped);

        if (maxLength > 60) {
            framesWithoutBoard = 0;

            // Periodic verification if already solved, or full extraction if not solved yet
            if (hasSolvedCache && (frameCounter % 12 == 0)) {
                Mat checkBoard, checkAlt, checkConf;
                int clues = extractSudokuDigits(undistorted, ohneGaus, maxLength, dr, checkBoard, checkAlt, checkConf, false);
                if (clues >= 14 && !boardsAreCompatible(checkBoard, cachedSolvedBoard)) {
                    // A different Sudoku board was placed in front of the camera;
                    // keep cachedSolvedCrop visible in top-right until the new one is solved!
                    hasSolvedCache = false;
                    std::memset(voteBuffer, 0, sizeof(voteBuffer));
                }
            }

            bool justSolved = false;
            if (!hasSolvedCache) {
                Mat sudokuMat, altSudokuMat, confidenceMat;
                int clues = extractSudokuDigits(undistorted, ohneGaus, maxLength, dr,
                                                sudokuMat, altSudokuMat, confidenceMat, false);
                lastCluesCount = clues;

                // Minimum 15 clues required to consider it a valid Sudoku board
                if (clues >= 15) {
                    // 1. Try solving directly from the current sharp frame
                    Mat solvedCandidate = sudokuMat.clone();
                    if (resolveAndSolveSudoku(solvedCandidate, altSudokuMat, confidenceMat)) {
                        cachedOrigBoard = sudokuMat.clone();
                        cachedSolvedBoard = solvedCandidate.clone();
                        cachedSolvedCrop = createSolvedBoardCrop(colorWarped, cachedOrigBoard, cachedSolvedBoard);
                        hasSolvedCache = true;
                        justSolved = true;
                        cout << "Sudoku detectado (" << clues << " pistas):\n" << cachedOrigBoard << "\n\n";
                        cout << "Sudoku resuelto:\n" << cachedSolvedBoard << "\n\n";
                    } else {
                        // 2. Accumulate multi-frame OCR votes for temporal robustness
                        for (int r = 0; r < 9; r++) {
                            for (int c = 0; c < 9; c++) {
                                uchar d = sudokuMat.at<uchar>(r, c);
                                if (d >= 1 && d <= 9) {
                                    voteBuffer[r][c][d]++;
                                }
                            }
                        }

                        Mat consensusBoard = Mat::zeros(Size(9, 9), CV_8UC1);
                        int consensusClues = 0;
                        for (int r = 0; r < 9; r++) {
                            for (int c = 0; c < 9; c++) {
                                int bestD = 0, bestVotes = 1; // Require at least 2 votes
                                for (int d = 1; d <= 9; d++) {
                                    if (voteBuffer[r][c][d] > bestVotes) {
                                        bestVotes = voteBuffer[r][c][d];
                                        bestD = d;
                                    }
                                }
                                if (bestD > 0) {
                                    consensusBoard.at<uchar>(r, c) = (uchar)bestD;
                                    consensusClues++;
                                }
                            }
                        }

                        if (consensusClues >= 17) {
                            Mat consensusSolved = consensusBoard.clone();
                            if (resolveAndSolveSudoku(consensusSolved, altSudokuMat, confidenceMat)) {
                                cachedOrigBoard = consensusBoard.clone();
                                cachedSolvedBoard = consensusSolved.clone();
                                cachedSolvedCrop = createSolvedBoardCrop(colorWarped, cachedOrigBoard, cachedSolvedBoard);
                                hasSolvedCache = true;
                                justSolved = true;
                                cout << "Sudoku detectado por consenso (" << consensusClues << " pistas):\n" << cachedOrigBoard << "\n\n";
                                cout << "Sudoku resuelto:\n" << cachedSolvedBoard << "\n\n";
                            }
                        }
                    }
                }
            }

            if (hasSolvedCache) {
                overlaySolvedNumbers(frame, cachedOrigBoard, cachedSolvedBoard, maxLength, invTransform);
                drawSolvedCropPiP(frame, cachedSolvedCrop, smoothedQuad, 46, true);
                drawStatusBanner(frame, "SUDOKU RESUELTO | [R] Re-escanear | [S] Guardar | [Q] Salir", E2I_PRIMARY_MINT);

                if (justSolved) {
                    imwrite(outputPath, frame);
                    cout << "Imagen resuelta capturada y guardada en: " << outputPath << "\n";
                }
            } else {
                if (!cachedSolvedCrop.empty()) {
                    drawSolvedCropPiP(frame, cachedSolvedCrop, nullptr, 46, false);
                }
                string msg = "Detectando digitos (" + std::to_string(lastCluesCount) + " pistas)... Mantenga estable";
                drawStatusBanner(frame, msg, E2I_SEC_MINT_LIGHT);
            }
        } else {
            framesWithoutBoard++;
            if (framesWithoutBoard > 15) {
                hasPrevQuad = false;
                if (framesWithoutBoard > 30) {
                    // Ready to recognize a new board when shown, but keep cachedSolvedCrop pinned in top-right!
                    hasSolvedCache = false;
                    std::memset(voteBuffer, 0, sizeof(voteBuffer));
                }
            }
            if (!cachedSolvedCrop.empty()) {
                drawSolvedCropPiP(frame, cachedSolvedCrop, nullptr, 46, false);
            }
            drawStatusBanner(frame, "Buscando Sudoku... Apunte la camara al tablero | [Q] Salir", E2I_SEC_GRAY);
        }

        if (simBaseImage && hasSolvedCache) {
            imwrite(outputPath, frame);
            break;
        }

        if (showGui) {
            imshow("Sudoku AR - Camara en Tiempo Real", frame);
            int key = waitKey(1) & 0xFF;
            if (key == 27 || key == 'q' || key == 'Q') {
                break;
            } else if (key == 'r' || key == 'R') {
                hasSolvedCache = false;
                hasPrevQuad = false;
                std::memset(voteBuffer, 0, sizeof(voteBuffer));
                cout << "Reiniciando deteccion de Sudoku...\n";
            } else if (key == 's' || key == 'S') {
                imwrite(outputPath, frame);
                cout << "Captura guardada en: " << outputPath << "\n";
            }
        }
    }

    if (showGui) {
        destroyAllWindows();
    }
    return 0;
}

int main(int argc, char *argv[])
{
    string imagePath = "";
    string outputPath = "../install/images/output.jpg";
    string simCameraImage = "";
    bool showGui = true;
    bool useCamera = false;
    int cameraId = 0;
    int posArg = 0;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--no-gui") {
            showGui = false;
        } else if (arg == "--camera" || arg == "-c") {
            useCamera = true;
            if (i + 1 < argc && std::isdigit(argv[i + 1][0])) {
                cameraId = std::stoi(argv[++i]);
            }
        } else if (arg == "--sim-camera" && i + 1 < argc) {
            simCameraImage = argv[++i];
        } else if (arg[0] != '-') {
            if ((useCamera || !simCameraImage.empty()) && posArg == 0) {
                outputPath = arg;
            } else if (posArg == 0) {
                imagePath = arg;
            } else if (posArg == 1) {
                outputPath = arg;
            }
            posArg++;
        }
    }

    DigitRecognizer dr;
    dr.train("../install/Character_Recognition/train-images.idx3-ubyte", "../install/Character_Recognition/train-labels.idx1-ubyte");

    // Simulated camera stream mode (for testing real-time pipeline on an image)
    if (!simCameraImage.empty()) {
        Mat simImg = imread(simCameraImage, IMREAD_COLOR);
        if (!simImg.data) {
            cout << "Could not open image for camera simulation: " << simCameraImage << std::endl;
            return -1;
        }
        return runRealtimeStream(nullptr, &simImg, outputPath, dr, showGui);
    }

    // Real-time webcam mode (when --camera / -c is passed, or when run with no arguments)
    if (useCamera || imagePath.empty()) {
        VideoCapture cap;
        cap.open(cameraId, cv::CAP_AVFOUNDATION);
        if (!cap.isOpened()) {
            cap.open(cameraId);
        }
        if (!cap.isOpened()) {
            cout << "No se pudo abrir la camara (ID " << cameraId << "). Verifica los permisos de camara en macOS.\n";
            return -1;
        }
        cap.set(CAP_PROP_FRAME_WIDTH, 1280);
        cap.set(CAP_PROP_FRAME_HEIGHT, 720);
        return runRealtimeStream(&cap, nullptr, outputPath, dr, showGui);
    }

    // Static image mode
    Mat input = imread(imagePath, IMREAD_COLOR);
    if (!input.data) {
        cout << "Could not open or find the image: " << imagePath << std::endl;
        return -1;
    }

    Mat undistorted;
    Mat ohneGaus;
    Mat invTransform;
    Mat colorWarped;
    Point2f detectedQuad[4];

    int maxLength = getBox(input, undistorted, ohneGaus, &invTransform,
                           detectedQuad, nullptr, /*allowHoughFallback=*/true, &colorWarped);
    if (maxLength <= 0) {
        cout << "No se detecto un tablero de Sudoku en la imagen.\n";
        return -1;
    }

    Mat sudokuMat, altSudokuMat, confidenceMat;
    extractSudokuDigits(undistorted, ohneGaus, maxLength, dr, sudokuMat, altSudokuMat, confidenceMat, showGui);

    cout << " Sudoku detectado:\n" << sudokuMat << "\n\n";

    Mat solvedMat = sudokuMat.clone();
    if (resolveAndSolveSudoku(solvedMat, altSudokuMat, confidenceMat)) {
        cout << "Sudoku resuelto:\n" << solvedMat << "\n";
        Mat solvedCrop = createSolvedBoardCrop(colorWarped, sudokuMat, solvedMat);
        overlaySolvedNumbers(input, sudokuMat, solvedMat, maxLength, invTransform);
        drawSolvedCropPiP(input, solvedCrop, detectedQuad, 14, true);
    } else {
        cout << "No se encontro solucion valida para el Sudoku detectado.\n";
    }

    imwrite(outputPath, input);

    if (showGui) {
        destroyWindow("roi");
        imshow("Original", input);
        waitKey(0);
        destroyAllWindows();
    }

    return 0;
}



