#!/bin/bash

cleanup() {
    echo echo "SIGINT signal caught. Deactivating the virtual environment and terminating the application."
    deactivate  
    exit 0      
}

trap cleanup SIGHUP SIGINT EXIT

if [ -d ".venv" ]; then
    echo "Folder .venv exists."
else
    python -m venv .venv
    if [ -f "requirements.txt" ]; then
    pip install -r requirements.txt
    else
        echo "Requirements file not found."
    fi

fi
source .venv/Scripts/activate
echo -e "\n\nClose Terminal: CTRL+C\n\n"
streamlit run App/App.py


