import {
     Box,
     FormControl,
     InputLabel,
     Select,
     MenuItem
 } from "@mui/material"
import { useState } from "react"

export const Charts = () => {
    const [hive, setHive] = useState(1);

    const handleSelectHive = (event) => {
        // make request to backend to get new data for selected hive
        setHive(event.target.value)
        console.log('<<=|X|=>> ~ file: Charts.js ~ line 16 ~ handleSelectHive ~ event.target.value', event.target.value);
    }

    return (
        <Box>
            <FormControl fullWidth>
                <InputLabel id="demo-simple-select-label">Hive</InputLabel>
                <Select
                    labelId="demo-simple-select-label"
                    id="demo-simple-select"
                    value={hive}
                    label="Age"
                    onChange={handleSelectHive}
                >
                    <MenuItem value={1}>One</MenuItem>
                    <MenuItem value={2}>Two</MenuItem>
                    <MenuItem value={3}>Three</MenuItem>
                    <MenuItem value={4}>Four</MenuItem>
                    <MenuItem value={5}>Five</MenuItem>
                    <MenuItem value={6}>Six</MenuItem>
                    <MenuItem value={7}>Seven</MenuItem>
                    <MenuItem value={8}>Eight</MenuItem>
                </Select>
            </FormControl>
        </Box>
    )
}