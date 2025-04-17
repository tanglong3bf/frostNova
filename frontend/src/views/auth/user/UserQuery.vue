<script lang="ts" setup>
import { ref } from 'vue'
import { Search, Refresh } from '@element-plus/icons-vue'
import type { FormInstance } from 'element-plus'
import '@/api/types/index.d.ts'

const queryForm = ref<FormInstance>()

const props = defineProps({
  /** 查询条件 */
  queryParams: {
    type: Object as () => UserQuery,
    required: true,
  },
})

const emits = defineEmits(['getList', 'resetQuery'])

const getList = () => {
  emits('getList')
}

const resetQuery = () => {
  emits('resetQuery')
}

/** 按条件查询 */
const handleQuery = async (form: FormInstance | undefined) => {
  if (!form) {
    return
  }
  await form.validate((valid, fields) => {
    if (!valid) {
      return
    }
    getList()
  })
}

const rules = {
  username: [
    {
      validator: (rule: any, value: any, callback: any) => {
        const reg = /^[a-zA-Z0-9_]*$/
        if (reg.test(value)) {
          callback()
        }
        else {
          callback(new Error('用户名只能包含字母、数字、下划线'))
        }
      }, trigger: 'blur'
    },
  ],
  phoneNumber: [
    {
      validator: (rule: any, value: any, callback: any) => {
        const reg = /^\d{0,11}$/
        if (value === undefined || reg.test(value)) {
          callback()
        }
        else {
          callback(new Error('电话号码只能包含至多11位数字'))
        }
      }, trigger: 'blur'
    },
  ],
}
</script>

<template>
  <el-form :model="queryParams" ref="queryForm" size="default" :rules="rules" :inline="true" label-width="68px">
    <el-form-item label="用户名称" prop="username">
      <el-input v-model="queryParams.username" placeholder="请输入用户名称" clearable style="width: 240px"
        @keyup.enter.native="handleQuery" />
    </el-form-item>
    <el-form-item label="手机号码" prop="phoneNumber">
      <el-input v-model="queryParams.phoneNumber" placeholder="请输入手机号码" clearable style="width: 240px"
        @keyup.enter.native="handleQuery" />
    </el-form-item>
    <el-form-item label="状态" prop="status">
      <el-select v-model="queryParams.status" placeholder="用户状态" clearable style="width: 240px">
        <el-option label="正常" :value="0" />
        <el-option label="禁用" :value="1" />
      </el-select>
    </el-form-item>
    <el-form-item label="创建时间" prop="dateRange">
      <el-date-picker v-model="queryParams.dateRange" style="width: 240px" value-format="YYYY-MM-DD" type="daterange"
        range-separator="-" start-placeholder="开始日期" end-placeholder="结束日期"></el-date-picker>
    </el-form-item>
    <el-form-item>
      <el-button type="primary" :icon="Search" size="small" @click="handleQuery(queryForm)">搜索</el-button>
      <el-button :icon="Refresh" size="small" @click="resetQuery()">重置</el-button>
    </el-form-item>
  </el-form>
</template>