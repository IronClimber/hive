export async function getDataAPI() {
    const response = await fetch('http://127.0.0.1:8000/monitoring/last/json/5')
    
    if (!response.ok) {
      throw new Error('Network response was not ok')
    }

    return response.json()
}